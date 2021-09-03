#pragma once

#include "OrderBook.h"

#include <set>
#include <unordered_set>
#include <map>
#include <string>

namespace stl {

enum class ORDER_TYPE {ASK, BID};

using OrderID = uint64_t;
using Price = uint64_t;
using Qty = uint64_t;

// data related to each order type
struct Orders
{
    // unique ids in order
    std::set<OrderID> by_id_;

    // prices count of offer for those prices in order
    std::map<Price, Qty> by_price_;

    // provides O(1) check speed or O(n) in worse
    std::unordered_set<Price> unordered_price_;

    // name of order type as string
    std::string order_type;

    // type of order as enum
    ORDER_TYPE type;
};

} // namespace stl
