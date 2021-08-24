#pragma once

#include <unordered_map>
#include <queue>
#include <vector>
#include <cstdint>


class Level2HashTableBase {
    using Price = uint64_t;     // price in cents but i guess i need to switch to something bigger to remove overflow issue
    using OfferID = uint64_t;   // id of offer. It
    using Count = uint64_t;
private:
    // Price -> offers with this price
    std::unordered_map<Price, std::vector<std::pair<OfferID, Count>>> bids_;

    std::unordered_map<OfferID, Price> ids;     // order_id -> Price

    uint64_t current_offer_id {};
public:
    Level2HashTableBase();

    auto add_order(unsigned int quantity, Price price, bool isBid) -> OfferID;
    bool close_order(unsigned int quantity, OfferID id);


    bool get_offers_by_price(Price price, std::vector<std::pair<OfferID, Count>>*& vec);
    bool get_offers_by_id(OfferID id, std::pair<OfferID, Count>*& offer_id);


    void print_level2_by_price();
    void print_level2_by_idx();
    auto get_l2_size() -> OfferID;
};