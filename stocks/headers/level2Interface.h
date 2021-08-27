#pragma once

#include "level2HashTableBase.h"
#include "level2BinaryTreeBase.h"
#include "level2MultiIndexBase.h"

using Price = uint64_t;     // price in cents but i guess i need to switch to something bigger to remove overflow issue
using OfferID = uint64_t;   // id of offer. It
using Count = uint64_t;

using namespace binary_tree_base;
//using namespace hashtable_base;

class Level2Interface {
    //using level2 = Level2HashTableBase;
    using level2 = Level2BinaryTreeBase;
private:
    level2 l2;
public:

    bool get_offers_by_price(Price price, std::vector<OfferById>*& v) noexcept {
        return l2.get_offers_by_price(price, v);
    }

    bool get_offers_by_id(OfferID id, OfferByPrice*& v) noexcept {
        return l2.get_offers_by_id(id, v);
    }

    OfferID add_order(unsigned int quantity, Price price, OFFER offer_type) {
        return l2.add_order(quantity, price, offer_type);
    }

    bool close_order(unsigned int quantity, OfferID id) {
        return l2.close_order(quantity, id);
    }

    auto get_offers_by_price(Price price) -> std::vector<OfferById> {
        return l2.get_offers_by_price(price);
    }

    auto get_offers_by_id(OfferID id) -> OfferByPrice {
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

    bool store() {
        return l2.store();
    }

    bool load() {
        return l2.load();
    }
};
