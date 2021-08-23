#include "level2.h"

#include <iostream>

using Price = uint64_t;     // price in cents but i guess i need to switch to something bigger to remove overflow issue
using OfferID = uint64_t;   // id of offer. It
using Count = uint64_t;

#define print ;//std::cout << __FILE__ << ":" << __LINE__ << '\t' << __FUNCTION__ << '\n';

Level2::Level2() {

}

auto Level2::add_order(unsigned int quantity, Price price, bool isBid) -> OfferID {
    auto bids = bids_.find(price);
    current_offer_id++; // TODO: Check overflow
    ids.emplace(current_offer_id, price); // Вставить id оффера, чтобы быстро находить offer_id
    bids_[price].emplace_back(current_offer_id, quantity);// Вставить в хранилище баев price -> id, cnt
    return current_offer_id;
}

bool Level2::close_order(unsigned int quantity, OfferID id) {
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

bool Level2::get_offers_by_price(Price price, std::vector<std::pair<OfferID, Count>>*& vec) {
    auto bid = bids_.find(price);
    if (bid != bids_.end()) {
        vec = &(bid->second);
        return true;
    }
    return false;
    // asks has the same algo if no ... dunno return {} or ret value as argument and bool as status
}

bool Level2::get_offers_by_id(OfferID id, std::pair<OfferID, Count>*& offer_id) {
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
    for (int i = 0; i < offers->size(); i++) {
        if (offers->at(i).first == id) {
            offer_id = &(offers->at(i));
        }
    }
    return true;
}





void Level2::print_level2_by_price() {
    printf("bids:\n");
    for (const auto &offer: bids_) {
        for (int i = 0; i < offer.second.size(); i++) {
            printf("price: [%llu]; bid id: [%llu]; count: [%llu]\n", offer.first, offer.second[i].first, offer.second[i].second);
        }
    }
//    printf("asks:\n");
//    for (const auto &offer: asks_) {
//        for (int i = 0; i < offer.second.size(); i++) {
//            printf("price: [%llu]; bid id: [%llu]; count: [%llu]\n", offer.first, offer.second[i].first, offer.second[i].second);
//        }
//    }
}

void Level2::print_level2_by_idx() {
    printf("print by indexes:\n");
    for (const auto& id: ids) {
        std::pair<OfferID, Count>* offer;
        get_offers_by_id(id.first, offer);
        printf("id: [%llu], price: [%llu], count: [%llu]\n", id.first, id.second, offer->second);
    }
}

auto Level2::get_l2_size() -> OfferID {
    unsigned long long size {};

    for (const auto &bid: bids_) {
        auto offer = bid.second;
        for (const auto& b: offer) {
            size += b.second;
        }
    }

//    for (const auto &ask: asks_) {
//        auto offer = ask.second;
//        for (const auto& b: offer) {
//            size += b.second;
//        }
//    }
    return size;
}


