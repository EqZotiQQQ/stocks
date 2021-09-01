#include "OrderBook.h"

#include <fstream>
#include <iomanip>
#include <iostream>

OrderBookAbseil::OrderBookAbseil() {
    asks_.type_ = "ask";
    bids_.type_ = "bid";
}

OfferId OrderBookAbseil::add_order(Offer order_type, Price price, Qty quantity) {
    if (order_type == Offer::ASK) {
        quantity = exchange_orders(bids_, price, quantity);
        add_order_to(asks_, price, quantity, offer_id_);
    } else {
        quantity = exchange_orders(asks_, price, quantity);
        add_order_to(bids_, price, quantity, offer_id_);
    }
    return offer_id_++;
}

void OrderBookAbseil::add_order_to(Orders& orders, Price price, Qty quantity, OfferId id) {
    if (quantity != 0u) {
        orders.by_id_.insert(id);
        if (price_to_id_.contains(price)) {
            orders.by_price_[price]++;
            price_to_id_[price].insert(id);
        } else {
            orders.by_price_.emplace(price, 1);
            price_to_id_.insert({price, absl::btree_set<OfferId>{id}});
        }
        offers_data_.emplace(id, PriceQty{.price = price, .qty = quantity});
        order_size_ += quantity;
    }
}

Qty OrderBookAbseil::exchange_orders(Orders& orders, Price order_price, Qty qty) {
    if (qty == 0 || orders.by_id_.empty()) {
        return qty;
    }

    Price target_price;
    OfferId target_id;
    Qty target_qty;

    bool exchange_condition;

    if (orders.type_ == "bid") {
        target_price =   bids_.by_price_.begin()->first;
        target_id =      *price_to_id_[target_price].begin();
        target_qty =     offers_data_[target_id].qty;
        exchange_condition = order_price >= target_price;
    } else {
        target_price =   asks_.by_price_.begin()->first;
        target_id =      *price_to_id_[target_price].begin();
        target_qty =     offers_data_[target_id].qty;
        exchange_condition = target_price >= order_price;
    }

    if (exchange_condition) {
        if (offers_data_[target_id].qty > qty) {
            offers_data_[target_id].qty -= qty;
            order_size_ -= qty;
        } else {
            orders.by_id_.erase(target_id);
            qty -= target_qty;
            offers_data_.erase(target_id);
            if (--orders.by_price_[target_price] == 0) {
                orders.by_price_.erase(target_price);
                price_to_id_.erase(target_price);
            }
            order_size_ -= target_qty;
        }
    } else {
        return qty;
    }
    return exchange_orders(orders, order_price, qty);
}

absl::flat_hash_map<OfferId, PriceQty> OrderBookAbseil::pack_all_data() {
    absl::flat_hash_map<OfferId, PriceQty> ret;
    for (auto id: offers_data_) {
        PriceQty price_qty {.price=id.second.price, .qty=id.second.qty};
        ret.emplace(id.first, price_qty);
    }
    return ret;
}

bool OrderBookAbseil::close_order(OfferId id, Qty quantity) {
    if (offers_data_.contains(id)) {
        if (quantity > offers_data_[id].qty) {
            printf("Failed to close %llu order. Available %llu while you tried to close %llu.\n", id, offers_data_[id].qty, quantity);
            return false;
        }
        Orders& orders = bids_.by_id_.contains(id) ? bids_: asks_;

        if (quantity == offers_data_[id].qty) {
            Price order_price = offers_data_[id].price;
            price_to_id_[order_price].erase(id);
            offers_data_.erase(id);
            orders.by_id_.erase(id);
            if (price_to_id_[order_price].empty()) {
                price_to_id_.erase(order_price);
                orders.by_price_.erase(order_price);
            } else {
                orders.by_price_[order_price]--;
            }
        } else {
            offers_data_[id].qty -= quantity;
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

bool OrderBookAbseil::store(const std::string& name) {
    std::ofstream file(name);
    if (!file.is_open()) {
        return false;
    }
    nlohmann::json json;
    auto damn = {&bids_, &asks_};
    for (const auto& offer: damn) {
        for (auto id_by_price = offer->by_price_.rbegin(); id_by_price != offer->by_price_.rend(); id_by_price++) {
            absl::btree_set<OfferId> ids_by_same_price = get_offers_by_price(id_by_price->first).value();
            for (auto id: ids_by_same_price) {
                nlohmann::json offers_by_price;
                offers_by_price[offer->type_]["id"] = id;
                offers_by_price[offer->type_]["price"] = offers_data_[id].price;
                offers_by_price[offer->type_]["quantity"] = offers_data_[id].qty;
                json += offers_by_price;
            }
        }
    }

    file << std::setw(4) << json << std::endl;
    return true;
}

bool OrderBookAbseil::load(const std::string& name) noexcept {
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

void OrderBookAbseil::create_offer_structure(const nlohmann::basic_json<>& json) noexcept {
    for (const auto& str : json) {
        for (const auto& m : str) {
            if (str.begin().key() == "bid") {
                add_order_to(bids_, m["price"], m["quantity"], m["id"]);
            } else {
                add_order_to(asks_, m["price"], m["quantity"], m["id"]);
            }
            offer_id_++;
        }
    }
}

void OrderBookAbseil::print_by_id() const {
    auto damn = {&bids_, &asks_};
    for (const auto& offer: damn) {
        for (const auto& id : offer->by_id_) {
            printf("order id: [%u]; price: [%u]; quantity: [%u]\n",
                   id, offers_data_.at(id).price, offers_data_.at(id).qty);
        }
    }
}


void OrderBookAbseil::print_by_price() const {
    auto damn = {&bids_, &asks_};
    for (const auto& offer: damn) {
        for (auto price_iter = offer->by_price_.begin(); price_iter != offer->by_price_.end(); price_iter++) {
            Price price = price_iter->first;
            absl::btree_set<OfferId> ids_by_price = price_to_id_.at(price);
            std::string ids = "";
            Qty total_qty_by_price{};
            for (OfferId id : ids_by_price) {
                ids = absl::StrFormat("%s[%u]", ids, id);
                total_qty_by_price += offers_data_.at(id).qty;
            }
            printf("Price: [%u]; Ids: [%s]; Qty: [%d]\n", price, ids.c_str(), total_qty_by_price);
        }
    }
}


absl::optional<PriceQty> OrderBookAbseil::get_order_by_id(OfferId id) const {
    if (offers_data_.contains(id)) {
        return PriceQty {.price = offers_data_.at(id).price, .qty = offers_data_.at(id).qty};
    }
    return {};
}

absl::optional<absl::btree_set<OfferId>> OrderBookAbseil::get_offers_by_price(Price price) const {
    if (price_to_id_.contains(price)) {
        return price_to_id_.at(price);
    }
    return {};
}
