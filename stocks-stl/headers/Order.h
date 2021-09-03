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

// Struct that contains data for each type
struct Orders
{
    // contains unique ids in order. Use them like keys
    std::set<OrderID> by_id_;
    // contain prices count of offer for those prices in order. Use them like keys
    std::map<Price, Qty> by_price_;
    // O(1) check speed. Return false if insert failed
    std::unordered_set<Price> unordered_price_;

    // name of order type as string
    std::string order_type;

    // type of order as enum
    ORDER_TYPE type;
};

} // namespace stl