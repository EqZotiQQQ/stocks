#include "level2HashTableBase.h"

#include <iostream>
#include <functional>
#include <map>
#include <numeric>

using Price = uint64_t;     // price in cents but i guess i need to switch to something bigger to remove overflow issue
using OfferID = uint64_t;   // id of offer. It
using Count = uint64_t;

#define print ;//std::cout << __FILE__ << ":" << __LINE__ << '\t' << __FUNCTION__ << '\n';

Level2HashTableBase::Level2HashTableBase() {

}

auto Level2HashTableBase::add_order(unsigned int quantity, Price price, bool isBid) -> OfferID {
    auto bids = bids_.find(price);
    current_offer_id++; // TODO: Check overflow



    ids.emplace(current_offer_id, price); // Вставить id оффера, чтобы быстро находить offer_id
    bids_[price].emplace_back(current_offer_id, quantity);// Вставить в хранилище баев price -> id, cnt
    return current_offer_id;
}

bool Level2HashTableBase::close_order(unsigned int quantity, OfferID id) {
    std::cout << "Order id to remove: " << id << "\t Quantity: " << quantity << std::endl;
    auto close_pos = ids.find(id);

    if (close_pos == ids.end()) {
        printf("No offers with this id\n");
        return false;
    }

    std::pair<OfferID, Count>* offer = nullptr;
    if (get_offers_by_id(id, offer)) {
        if (quantity > offer->second) {
            printf("Failed to close order. Incorrect count. You tried to close %d offers when available only %llu\n",
                   quantity, offer->second);
            return false;
        }
        offer->second -= quantity;  // TODO: check overflow/under: use protected methods!
        if (!offer->second) {
            ids.erase(id);
        }
    } else {
        std::cout << "Invalid offer: " << offer << '\n';
        return false;
    }
    return true;
}

bool Level2HashTableBase::get_offers_by_price(Price price, std::vector<std::pair<OfferID, Count>>*& vec) {
    auto bid = bids_.find(price);
    if (bid != bids_.end()) {
        vec = &(bid->second);
        return true;
    }
    return false;
    // asks has the same algo if no ... dunno return {} or ret value as argument and bool as status
}

bool Level2HashTableBase::get_offers_by_id(OfferID id, std::pair<OfferID, Count>*& offer_id) {
    auto price = ids.find(id);
    if (price == ids.end()) {
        std::cout << "no such id in level2\n";
        return false;
    }
    std::vector<std::pair<OfferID, Count>> *offers = nullptr;
    if (!get_offers_by_price(price->second, offers)) {
        std::cout << "no such offer in level2\n";
        return false;
    }
    for (auto& offer : *offers) {
        if (offer.first == id) {
            offer_id = &offer;
            return true;
        }
    }
    return false;
}





void Level2HashTableBase::print_level2_by_price() {
    printf("print ordered by price:\n");
    std::map<Price, std::vector<std::pair<OfferID, Count>>> ordered(bids_.begin(), bids_.end());
    for (auto & it : ordered) {
        for (auto it2 = it.second.begin(); it2 != it.second.end(); it2++) {
            printf("Price: [%llu], Offer: [%llu], Count: [%llu]\n", it.first, it2->first, it2->second);
        }
    }
}

void Level2HashTableBase::print_level2_by_idx() {
    printf("print ordered by indexes:\n");
    for (const auto& id: ids) {
        std::pair<OfferID, Count>* offer;
        get_offers_by_id(id.first, offer);
        printf("id: [%llu], price: [%llu], count: [%llu]\n", id.first, id.second, offer->second);
    }
}

auto Level2HashTableBase::get_l2_size() -> OfferID {
    unsigned long long size {};

    for (const auto &bid: bids_) {
        auto offer = bid.second;
        for (const auto& b: offer) {
            size += b.second;
        }
    }
    return size;
}


