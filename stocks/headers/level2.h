#pragma once

#include <unordered_map>
#include <queue>

class Level2 {
    using Price = double;
    using OfferID = unsigned int;
private:
    std::unordered_map<Price, std::queue<OfferID>> bids;     // Price -> offers with this price
    std::unordered_map<Price, std::queue<OfferID>> asks;
public:
    Level2();
    void add_order(unsigned int quantity);
    void close_order(unsigned int quantity);
};