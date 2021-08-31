#pragma once

#include <absl/container/btree_map.h>
#include <absl/container/btree_set.h>
#include <absl/container/flat_hash_map.h>
#include <absl/container/flat_hash_set.h>
#include "absl/strings/str_format.h"

using OfferId = uint64_t;
using Price = uint64_t;
using Qty = uint64_t;

enum class Offer {
    BID,
    ASK,
};

struct PriceQty {
    Price price {};
    Qty qty {};
    Offer type;
};


class OrderBookAbseil {
private:
    // contains unique ids in order. Use them like keys
    absl::btree_set<OfferId> bids_ordered_by_offer_id_;
    absl::btree_set<OfferId> asks_ordered_by_offer_id_;

    // contain prices count of offer for those prices in order. Use them like keys
    absl::btree_map<Price, Qty> bids_ordered_by_price_;
    absl::btree_map<Price, Qty> asks_ordered_by_price_;

    // provides O(1) access to count
    absl::flat_hash_map<OfferId, PriceQty> offers_sorted_by_id_;

    // provides O(1) access to id
    absl::flat_hash_map<Price, std::vector<OfferId>> price_to_id_;


    OfferId offer_id_ {};

    void add_order_to(Offer offer_type, Price price, Qty quantity, OfferId id);
    Qty exchange_orders(Offer offer_type, Price offer_price, Qty offer_quantity);
public:
    OfferId add_order(Offer offer_type, Price price, Qty quantity);

    void print_offers_ordered_by_id() const;
    void print_offers_ordered_by_price();
};
