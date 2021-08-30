#pragma once

#include <absl/container/node_hash_map.h>

using OfferId = uint64_t;
using Price = uint64_t;
using Qty = uint64_t;

struct PriceQty {
    Price price;
    Qty qty;
};

class OrderBookAbseil {
private:
    OfferId add_bid(Price price, Qty qty);
    OfferId add_ask(Price price, Qty qty);

public:
    absl::node_hash_map<OfferId, PriceQty> offers_;
    OfferId offer_id_;

};
