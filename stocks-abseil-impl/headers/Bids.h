#pragma once

#include <absl/container/btree_map.h>
#include <absl/container/btree_set.h>

using OfferId = uint64_t;
using Price = uint64_t;
using Qty = uint64_t;

struct Bids {
    absl::btree_map<Price, Qty> bids_ordered_by_price_;     // price -> number of offers by this price
    absl::btree_set<OfferId> bids_ordered_by_offer_id_;     // orders sorted by index
};