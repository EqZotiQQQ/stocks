#include "OrderBook.h"

#include <fstream>
#include <iomanip>
#include <iostream>

OfferId OrderBookAbseil::add_order(Offer offer_type, Price price, Qty quantity)
{
    quantity = exchange_orders(offer_type, price, quantity);
    add_order_to(offer_type, price, quantity, offer_id_);
    return offer_id_++;
}

void OrderBookAbseil::add_order_to(Offer order_type, Price price, Qty quantity, OfferId id)
{
    if (quantity != 0u) {
        if (order_type == Offer::ASK) {
            asks_.asks_ordered_by_offer_id_.insert(id);
            if (price_to_id_.contains(price)) {
                asks_.asks_ordered_by_price_[price]++;
                price_to_id_[price].push_back(id);
            } else {
                asks_.asks_ordered_by_price_.emplace(price, 1);
                price_to_id_.insert({price, std::vector<OfferId>{id}});
            }
        } else {
            bids_.bids_ordered_by_offer_id_.insert(id);
            if (price_to_id_.contains(price)) {
                bids_.bids_ordered_by_price_[price]++;
                price_to_id_[price].push_back(id);
            } else {
                bids_.bids_ordered_by_price_.emplace(price, 1);
                price_to_id_.insert({price, std::vector<OfferId>{id}});
            }
        }
        offers_data_.emplace(offer_id_, PriceQty{.price=price, .qty=quantity, .type=order_type});
    }
}

Qty OrderBookAbseil::exchange_orders(Offer order_type, Price order_price, Qty qty)
{
    if (qty == 0) {
        return qty;
    }

    if (order_type == Offer::BID) {
        if (asks_.asks_ordered_by_offer_id_.empty()) {
            return qty;
        }
        Price expensivest_ask_price = asks_.asks_ordered_by_price_.begin()->first;
        if (expensivest_ask_price >= order_price) {
            OfferId expensivest_ask_id = price_to_id_[expensivest_ask_price].front();
            Qty expensivest_ask_qty = offers_data_[expensivest_ask_id].qty;
            if (offers_data_[expensivest_ask_id].qty > qty) {
                offers_data_[expensivest_ask_id].qty -= qty;
            } else {
                Qty order_by_price_cnt = --asks_.asks_ordered_by_price_[expensivest_ask_price];
                asks_.asks_ordered_by_offer_id_.erase(expensivest_ask_id);
                qty -= expensivest_ask_qty;
                offers_data_.erase(expensivest_ask_id);
                if (order_by_price_cnt == 0) {
                    asks_.asks_ordered_by_price_.erase(expensivest_ask_price);
                    price_to_id_.erase(expensivest_ask_price);
                }
            }
        } else {
            return qty;
        }
    } else {
        if (bids_.bids_ordered_by_offer_id_.empty()) {
            return qty;
        }
        Price cheapest_bid_price = bids_.bids_ordered_by_price_.begin()->first;
        if (order_price >= cheapest_bid_price) {
            OfferId cheapest_bid_id = price_to_id_[cheapest_bid_price].front();;
            Qty cheapest_bid_qty = offers_data_[cheapest_bid_id].qty;

            if (offers_data_[cheapest_bid_id].qty > qty) {
                offers_data_[cheapest_bid_id].qty -= qty;
            } else {
                int s = bids_.bids_ordered_by_price_[cheapest_bid_price];
                Qty order_by_price_cnt = --bids_.bids_ordered_by_price_[cheapest_bid_price];
                bids_.bids_ordered_by_offer_id_.erase(cheapest_bid_id);
                qty -= cheapest_bid_qty;
                offers_data_.erase(cheapest_bid_id);
                if (order_by_price_cnt == 0) {
                    bids_.bids_ordered_by_price_.erase(cheapest_bid_price);
                    price_to_id_.erase(cheapest_bid_price);
                }
            }
        } else {
            return qty;
        }
    }
    return exchange_orders(order_type, order_price, qty);
}

void OrderBookAbseil::print_offers_ordered_by_id() const {
    for (auto bid_id: bids_.bids_ordered_by_offer_id_) {
        printf("order id: [%u]; price: [%u]; quantity: [%u]\n",
                                     bid_id,
                                     offers_data_.at(bid_id).price,
                                     offers_data_.at(bid_id).qty);
    }
    for (auto ask_id: asks_.asks_ordered_by_offer_id_) {
        printf("order id: [%u]; price: [%u]; quantity: [%u]\n",
                                     ask_id,
                                     offers_data_.at(ask_id).price,
                                     offers_data_.at(ask_id).qty);
    }
}

void OrderBookAbseil::print_offers_ordered_by_price() {
    for (auto price_iter = bids_.bids_ordered_by_price_.begin(); price_iter != bids_.bids_ordered_by_price_.end(); price_iter++) {
        Price price = price_iter->first;
        std::vector<OfferId> v = price_to_id_.at(price);
        std::string s = "";
        Qty qty {};
        for (OfferId id: v) {
            s = absl::StrFormat("%s %u", s, id);
            qty += offers_data_[id].qty;
        }
        printf("Price: [%u]; Ids: [%s]; Qty: [%d]\n", price, s.c_str(), qty);
    }

    for (auto price_iter = asks_.asks_ordered_by_price_.begin(); price_iter != asks_.asks_ordered_by_price_.end(); price_iter++) {
        Price price = price_iter->first;

        std::vector<OfferId> v = price_to_id_.at(price);
        std::string s = "";
        Qty qty {};
        for (Price id: v) {
            s = absl::StrFormat("%s[%u]", s, id);
            qty += offers_data_[id].qty;
        }
        printf("Price: [%u]; Ids: [%s]; Qty: [%d]\n", price, s.c_str(), qty);
    }
}
absl::flat_hash_map<OfferId, PriceQty> OrderBookAbseil::pack_all_data()
{
    absl::flat_hash_map<OfferId, PriceQty> ret;
    for (auto id: offers_data_) {
        PriceQty price_qty {
            .price=id.second.price,
            .qty=id.second.qty
        };
        ret.emplace(id.first, price_qty);
    }
    return ret;
}
bool OrderBookAbseil::close_order(OfferId id, Qty quantity)
{
    if (offers_data_.contains(id)) {
        if (quantity > offers_data_[id].qty) {
            return false;
        }
        if (offers_data_[id].type == Offer::BID) {
//            if (quantity == offers_data_[id].qty) {
//                Qty order_by_price_cnt = --bids_.bids_ordered_by_price_[cheapest_bid_price];
//                bids_.bids_ordered_by_offer_id_.erase(cheapest_bid_id);
//                qty -= cheapest_bid_qty;
//                offers_data_.erase(cheapest_bid_id);
//                if (order_by_price_cnt == 0) {
//                    bids_.bids_ordered_by_price_.erase(cheapest_bid_price);
//                    price_to_id_.erase(cheapest_bid_price);
//                }
//            } else {
//
//            }
        }
    }
    return false;
}

//Qty OrderBookAbseil::get_l2_size() {
//    return size_;
//}
/*Count OrderBookAbseil::close_order_helper(OfferID id, OfferSupporter_t orders, Count offer_quantity)
{
    Price cheapest_order = orders.offers_ordered_by_price_.begin()->first;

    orders.offers_ordered_by_offer_id_.erase(id);
    Count remove = id_to_count_[id];
    __builtin_usubll_overflow(offer_quantity, remove, &offer_quantity);
    id_to_count_.erase(id);
    id_to_price_.erase(id);
    OfferID offers_for_price_left = --orders.offers_ordered_by_price_[cheapest_order];
    unordered_offer_id_.erase(id);
    if (offers_for_price_left == 0) {
        orders.unordered_offer_price_.erase(cheapest_order);
        price_to_id_.erase(cheapest_order);
        orders.offers_ordered_by_price_.erase(cheapest_order);
    } else {
        auto iter = price_to_id_[cheapest_order].begin();
        price_to_id_[cheapest_order].erase(iter);
    }
    return offer_quantity;
}

bool OrderBookAbseil::close_order(OfferID id, Count quantity) noexcept
{
    if (id_to_count_.count(id) == 0) {
        printf("No such offer [%llu] to remove.\n", id);
        return false;
    }
    if (quantity > id_to_count_[id]) {
        printf("Unable to remove [%llu] while only [%llu] available.\n", quantity, id_to_count_[id]);
        return false;
    }

    if (unordered_offer_id_.find(id) != unordered_offer_id_.end()) {
        OFFER offer_type = unordered_offer_id_[id];
        OfferSupporter_t orders = identify_offer_helper(offer_type);
        if (quantity == id_to_count_[id]) {
            close_order_helper(id, orders, quantity);
        } else {
            id_to_count_[id] -= quantity;
        }
    }
    return true;
}

Count OrderBookAbseil::get_l2_size() const noexcept
{
    Count ret{};
    for (auto qt : id_to_count_) {
        // maybe worth to return bool and size as parameter?
        __builtin_uaddll_overflow(ret, qt.second, &ret);
    }
    return ret;
}

bool OrderBookAbseil::store(const std::string& name) const noexcept
{
    std::ofstream file(name);
    if (!file.is_open()) {
        return false;
    }
    nlohmann::json json;

    Price prev{};
    for (auto price = bids_ordered_by_price_.rbegin(); price != bids_ordered_by_price_.rend(); price++) {
        if (prev == price->first) {
            continue;
        }
        auto price_iter = price_to_id_.at(price->first);
        for (auto id : price_iter) {
            nlohmann::json offers_by_price;
            offers_by_price["bid"]["id"] = id;
            offers_by_price["bid"]["price"] = id_to_price_.at(id);
            offers_by_price["bid"]["quantity"] = id_to_count_.at(id);
            json += offers_by_price;
        }
        prev = price->first;
    }
    prev = 0;
    for (auto price = asks_ordered_by_price_.rbegin(); price != asks_ordered_by_price_.rend(); price++) {
        if (prev == price->first) {
            continue;
        }
        auto price_iter = price_to_id_.at(price->first);
        for (auto id : price_iter) {
            nlohmann::json offers_by_price;
            offers_by_price["ask"]["id"] = id;
            offers_by_price["ask"]["price"] = id_to_price_.at(id);
            offers_by_price["ask"]["quantity"] = id_to_count_.at(id);
            json += offers_by_price;
        }
        prev = price->first;
    }

    file << std::setw(4) << json << std::endl;
    return true;
}

bool OrderBookAbseil::load(const std::string& name) noexcept
{
    std::ifstream file(name);
    if (!file.is_open()) {
        printf("Unable to open file %s\n", name.c_str());
        return false;
    }
    nlohmann::json json;
    try {
        file >> json;
    } catch (const nlohmann::detail::parse_error& e) {
        printf("Invalid file\n");
        return false;
    }
    create_offer_structure(json);
    return true;
}

void OrderBookAbseil::create_offer_structure(const nlohmann::basic_json<>& json) noexcept
{
    for (const auto& str : json) {
        for (const auto& m : str) {
            if (str.begin().key() == "bids") {
                add_offer_to(OFFER::BID, m["price"], m["quantity"], m["id"]);
            } else {
                add_offer_to(OFFER::BID, m["price"], m["quantity"], m["id"]);
            }
        }
    }
}

bool OrderBookAbseil::get_offers_by_price(Price price, std::set<OfferID>*& offers) noexcept
{
    if (price_to_id_.count(price) != 0u) {
        offers = &price_to_id_[price];
        return true;
    }
    return false;
}

std::pair<Price, Count> OrderBook::get_offer_by_id(OfferID id) const noexcept
{
    if (id_to_price_.count(id) != 0u) {
        return {id_to_price_.at(id), id_to_count_.at(id)};
    }
    return {};
}

std::map<OfferID, std::pair<Price, Count>> OrderBook::pack_all_data() const noexcept
{
    std::map<OfferID, std::pair<Price, Count>> ret;
    for (auto id : bids_ordered_by_offer_id_) {
        ret.emplace(id, std::make_pair(id_to_price_.at(id), id_to_count_.at(id)));
    }
    for (auto id : asks_ordered_by_offer_id_) {
        ret.emplace(id, std::make_pair(id_to_price_.at(id), id_to_count_.at(id)));
    }
    return ret;
}

OrderBookAbseil::OfferSupporter_t OrderBookAbseil::identify_offer_helper(OFFER offer_type, bool swap_offer_type) noexcept
{
    if (swap_offer_type) {
        if (offer_type == OFFER::BID) {
            offer_type = OFFER::ASK;
        } else {
            offer_type = OFFER::BID;
        }
    }

    if (offer_type == OFFER::BID) {
        return OfferSupporter_t{
            .offers_ordered_by_offer_id_ = bids_ordered_by_offer_id_,
            .offers_ordered_by_price_ = bids_ordered_by_price_,
            .unordered_offer_price_ = unordered_bid_price_,
            };
    } else {
        return OfferSupporter_t{
            .offers_ordered_by_offer_id_ = asks_ordered_by_offer_id_,
            .offers_ordered_by_price_ = asks_ordered_by_price_,
            .unordered_offer_price_ = unordered_ask_price_,
            };
    }
}*/
