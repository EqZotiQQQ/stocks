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
            asks_.by_offer_id_.insert(id);
            if (price_to_id_.contains(price)) {
                asks_.by_price_[price]++;
                price_to_id_[price].push_back(id);
            } else {
                asks_.by_price_.emplace(price, 1);
                price_to_id_.insert({price, std::vector<OfferId>{id}});
            }
        } else {
            bids_.by_offer_id_.insert(id);
            if (price_to_id_.contains(price)) {
                bids_.by_price_[price]++;
                price_to_id_[price].push_back(id);
            } else {
                bids_.by_price_.emplace(price, 1);
                price_to_id_.insert({price, std::vector<OfferId>{id}});
            }
        }
        offers_data_.emplace(id, PriceQty{.price=price, .qty=quantity, .type=order_type});
        order_size_ += quantity;
    }
}

Qty OrderBookAbseil::exchange_orders(Offer order_type, Price order_price, Qty qty)
{
    if (qty == 0) {
        return qty;
    }

    if (order_type == Offer::BID) {
        if (asks_.by_offer_id_.empty()) {
            return qty;
        }
        Price expensivest_ask_price = asks_.by_price_.begin()->first;
        if (expensivest_ask_price >= order_price) {
            OfferId expensivest_ask_id = price_to_id_[expensivest_ask_price].front();
            Qty expensivest_ask_qty = offers_data_[expensivest_ask_id].qty;
            if (offers_data_[expensivest_ask_id].qty > qty) {
                offers_data_[expensivest_ask_id].qty -= qty;

                order_size_ -= qty;
            } else {
                Qty order_by_price_cnt = --asks_.by_price_[expensivest_ask_price];
                asks_.by_offer_id_.erase(expensivest_ask_id);
                qty -= expensivest_ask_qty;
                offers_data_.erase(expensivest_ask_id);
                if (order_by_price_cnt == 0) {
                    asks_.by_price_.erase(expensivest_ask_price);
                    price_to_id_.erase(expensivest_ask_price);
                }
                order_size_ -= expensivest_ask_qty;
            }
        } else {
            return qty;
        }
    } else {
        if (bids_.by_price_.empty()) {
            return qty;
        }
        Price cheapest_bid_price = bids_.by_price_.begin()->first;
        if (order_price >= cheapest_bid_price) {
            OfferId cheapest_bid_id = price_to_id_[cheapest_bid_price].front();;
            Qty cheapest_bid_qty = offers_data_[cheapest_bid_id].qty;

            if (offers_data_[cheapest_bid_id].qty > qty) {
                offers_data_[cheapest_bid_id].qty -= qty;
                order_size_-= qty;
            } else {
                int s = bids_.by_price_[cheapest_bid_price];
                Qty order_by_price_cnt = --bids_.by_price_[cheapest_bid_price];
                bids_.by_price_.erase(cheapest_bid_id);
                qty -= cheapest_bid_qty;
                offers_data_.erase(cheapest_bid_id);
                if (order_by_price_cnt == 0) {
                    bids_.by_price_.erase(cheapest_bid_price);
                    price_to_id_.erase(cheapest_bid_price);
                }
                order_size_ -= cheapest_bid_qty;
            }
        } else {
            return qty;
        }
    }
    return exchange_orders(order_type, order_price, qty);
}

void OrderBookAbseil::print_offers_ordered_by_id() const {
    for (auto bid_id: bids_.by_offer_id_) {
        printf("order id: [%u]; price: [%u]; quantity: [%u]\n",
                                     bid_id,
                                     offers_data_.at(bid_id).price,
                                     offers_data_.at(bid_id).qty);
    }
    for (auto ask_id: asks_.by_offer_id_) {
        printf("order id: [%u]; price: [%u]; quantity: [%u]\n",
                                     ask_id,
                                     offers_data_.at(ask_id).price,
                                     offers_data_.at(ask_id).qty);
    }
}

void OrderBookAbseil::print_offers_ordered_by_price() {
    for (auto price_iter = bids_.by_price_.begin(); price_iter != bids_.by_price_.end(); price_iter++) {
        Price price = price_iter->first;
        std::vector<OfferId> v = price_to_id_.at(price);
        std::string s = "";
        Qty qty {};
        for (OfferId id: v) {
            s = absl::StrFormat("%s[%u]", s, id);
            qty += offers_data_[id].qty;
        }
        printf("Price: [%u]; Ids: [%s]; Qty: [%d]\n", price, s.c_str(), qty);
    }

    for (auto price_iter = asks_.by_price_.begin(); price_iter != asks_.by_price_.end(); price_iter++) {
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
            printf("Failed to close %llu order. Available %llu while you tried to close %llu.\n", id, offers_data_[id].qty, quantity);
            return false;
        }
        if (offers_data_[id].type == Offer::BID) {
            if (quantity == offers_data_[id].qty) {
                Price order_price = offers_data_[id].price;
                std::vector<OfferId> ids_by_price = price_to_id_[order_price];

                std::vector<OfferId> ids = price_to_id_[order_price];
                for (int i = 0; i < ids.size(); i++) {
                    if (id == ids[i]) {
                        price_to_id_[order_price].erase(price_to_id_[order_price].begin() + i);
                    }
                }

                offers_data_.erase(id);
                bids_.by_offer_id_.erase(id);
                if (price_to_id_[order_price].empty()) {
                    price_to_id_.erase(order_price);
                    bids_.by_price_.erase(order_price);
                } else {
                    bids_.by_price_[order_price]--;
                }
            } else {
                offers_data_[id].qty -= quantity;
            }
        } else {    //ASK
            if (quantity == offers_data_[id].qty) {
                Price order_price = offers_data_[id].price;
                std::vector<OfferId> ids_by_price = price_to_id_[order_price];

                std::vector<OfferId> ids = price_to_id_[order_price];
                for (int i = 0; i < ids.size(); i++) {
                    if (id == ids[i]) {
                        price_to_id_[order_price].erase(price_to_id_[order_price].begin() + i);
                    }
                }

                offers_data_.erase(id);
                asks_.by_offer_id_.erase(id);
                if (price_to_id_[order_price].empty()) {
                    price_to_id_.erase(order_price);
                    asks_.by_price_.erase(order_price);
                } else {
                    asks_.by_price_[order_price]--;
                }
            } else {
                offers_data_[id].qty -= quantity;
            }
        }
        order_size_ -= quantity;
    } else {
        printf("Id %llu not found\n", id);
        return false;
    }
    return true;
}

Qty OrderBookAbseil::get_l2_size() const {
    return order_size_;
}

absl::optional<PriceQty> OrderBookAbseil::get_order_by_id(OfferId id)
{
    if (offers_data_.contains(id)) {
        return PriceQty {
            .price = offers_data_[id].price,
            .qty = offers_data_[id].qty
        };
    }
    return {};
}

absl::optional<std::vector<OfferId>> OrderBookAbseil::get_offers_by_price(Price price)
{
    if (price_to_id_.contains(price)) {
        return price_to_id_[price];
    }
    return {};
}

bool OrderBookAbseil::store(const std::string& name)
{
    std::ofstream file(name);
    if (!file.is_open()) {
        return false;
    }
    nlohmann::json json;

    for (auto id = bids_.by_price_.rbegin(); id != bids_.by_price_.rend(); id++) {
        std::vector<OfferId> ids = get_offers_by_price(id->first).value();
        for (int i = 0; i < id->second; i++) {
            nlohmann::json offers_by_price;
            offers_by_price["bid"]["id"] = ids[i];
            offers_by_price["bid"]["price"] = offers_data_[ids[i]].price;
            offers_by_price["bid"]["quantity"] = offers_data_[ids[i]].qty;
            json += offers_by_price;
        }
    }

    for (auto id = asks_.by_price_.rbegin(); id != asks_.by_price_.rend(); id++) {
        std::vector<OfferId> ids = get_offers_by_price(id->first).value();
        for (int i = 0; i < id->second; i++) {
            nlohmann::json offers_by_price;
            offers_by_price["ask"]["id"] = ids[i];
            offers_by_price["ask"]["price"] = offers_data_[ids[i]].price;
            offers_by_price["ask"]["quantity"] = offers_data_[ids[i]].qty;
            json += offers_by_price;
        }
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
            if (str.begin().key() == "bid") {
                add_order_to(Offer::BID, m["price"], m["quantity"], m["id"]);
            } else {
                add_order_to(Offer::ASK, m["price"], m["quantity"], m["id"]);
            }
            offer_id_++;
        }
    }
}
