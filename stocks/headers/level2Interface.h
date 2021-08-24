#pragma once

#include "level2HashTableBase.h"
#include "level2BinaryTreeBase.h"

class Level2Interface {
    //using level2 = Level2HashTableBase;
    using level2 = Level2BinaryTreeBase;

    using Price = uint64_t;     // price in cents but i guess i need to switch to something bigger to remove overflow issue
    using OfferID = uint64_t;   // id of offer. It
    using Count = uint64_t;

private:
    level2 l2;
public:
    auto add_order(unsigned int quantity, Price price, bool is_bid) -> OfferID {
        return l2.add_order(quantity, price, is_bid);
    }

    bool close_order(unsigned int quantity, OfferID id) {
        return l2.close_order(quantity, id);
    }

    bool get_offers_by_price(Price price, std::vector<std::pair<OfferID, Count>>*& vec) {
        return l2.get_offers_by_price(price, vec);
    }

    auto get_offers_by_price(Price price) -> std::vector<std::pair<OfferID, Count>> {
        return l2.get_offers_by_price(price);
    }

    bool get_offers_by_id(OfferID id, std::pair<OfferID, Count>*& offer_id) {
        return l2.get_offers_by_id(id, offer_id);
    }

    auto get_offers_by_id(OfferID id) -> std::pair<OfferID, Count> {
        return l2.get_offers_by_id(id);
    }

    void print_level2_by_price() {
        l2.print_level2_by_price();
    }

    void print_level2_by_idx() {
        l2.print_level2_by_idx();
    }

    auto get_l2_size() -> OfferID {
        return l2.get_l2_size();
    }
};