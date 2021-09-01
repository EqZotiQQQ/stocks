#pragma once

#include "Order.h"
#include "absl/strings/str_format.h"

#include <absl/container/btree_map.h>
#include <absl/container/btree_set.h>
#include <absl/container/flat_hash_map.h>
#include <absl/container/flat_hash_set.h>
#include <absl/types/optional.h>
#include <nlohmann/json.hpp>

enum class Offer {
    BID,
    ASK,
};

struct PriceQty {
    Price price {};
    Qty qty {};

    bool operator==(const PriceQty& rhs) const {
        return price == rhs.price && qty == rhs.qty;
    }
};


class OrderBookAbseil {
private:
    Orders bids_, asks_;

    // provides O(1) access to count
    absl::flat_hash_map<OfferId, PriceQty> offers_data_;    // id -> full order info

    // provides O(1) access to id
    absl::flat_hash_map<Price, absl::btree_set<OfferId>> price_to_id_;  // price -> ids by this price
//    absl::flat_hash_map<Price, std::vector<OfferId>> price_to_id_;  // price -> ids by this price


    OfferId offer_id_ {};
    Qty order_size_ {};

    void add_order_to(Orders& orders, Price price, Qty quantity, OfferId id);
    Qty exchange_orders(Orders& orders, Price offer_price, Qty offer_quantity);
    void create_offer_structure(const nlohmann::basic_json<>& json) noexcept;
public:
    OrderBookAbseil();
    OfferId add_order(Offer offer_type, Price price, Qty quantity);
    bool close_order(OfferId id, Qty quantity);
    Qty get_l2_size() const;

    absl::optional<PriceQty> get_order_by_id(OfferId id) const;
    absl::optional<absl::btree_set<OfferId>> get_offers_by_price(Price price) const;

    bool store(const std::string& name = "abseil_order_book_data.json");
    bool load(const std::string& name = "abseil_order_book_data.json") noexcept;

    void print_by_id() const;
    void print_by_price() const;
    absl::flat_hash_map<OfferId, PriceQty> pack_all_data();
};
