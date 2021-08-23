#include "level2.h"

#include <iostream>

using Price = uint64_t;     // price in cents but i guess i need to switch to something bigger to remove overflow issue
using OfferID = uint64_t;   // id of offer. It
using Count = uint64_t;

#define print ;//std::cout << __FILE__ << ":" << __LINE__ << '\t' << __FUNCTION__ << '\n';

Level2::Level2() {
    print
}

auto Level2::add_order(unsigned int quantity, Price price) -> OfferID {
    print
    auto bids = bids_.find(price);

    current_offer_id++;

    ids.emplace(current_offer_id, price); // Вставить id оффера, чтобы быстро находить offer_id

    // Вставить в хранилище баев price -> id, cnt
    bids_[price].emplace_back(current_offer_id, quantity);

    return current_offer_id;
}

bool Level2::close_order(unsigned int quantity, OfferID id) {
    print

    auto close_pos = ids.find(id);

    if (close_pos == ids.end()) {
        printf("No offers with this id\n");
        return false;
    }

    if (quantity > bids_[close_pos->second].size()) {
        printf("Failed to close order. Incorrect count\n");
        return false;
    }

    std::pair<OfferID, Count> offer;
    if (get_offer_by_id(id, offer)) {
        std::cout << "#1\t" << offer.second << '\n';
        offer.first -= quantity;
        if (!offer.first) {
            ids.erase(id);
        }
    }


    return true;
}

auto Level2::get_l2_size() -> OfferID {
    unsigned long long size {};

    for (const auto &bid: bids_) {
        auto offer = bid.second;
        for (const auto& b: offer) {
            size += b.second;
        }
    }

    for (const auto &ask: asks_) {
        auto offer = ask.second;
        for (const auto& b: offer) {
            size += b.second;
        }
    }
    return size;
}

bool Level2::get_offers_by_price(Price price, std::vector<std::pair<OfferID, Count>>* vec) {
    auto bid = bids_.find(price);
    if (bid != bids_.end()) {
        vec = &(bid->second);
        std::cout << "#3 vec\t" << vec->size() << "\tptr = " << vec << '\n';
        std::cout << "#3 bid\t" << bid->second.size() << "\tptr = " << &(bid->second) << '\n';
        return true;
    }
    return false;
    // asks has the same algo if no ... dunno return {} or ret value as argument and bool as status
}

bool Level2::get_offer_by_id(OfferID id, std::pair<OfferID, Count>& offerid) {
    std::pair<OfferID, Count> ret{};
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
    std::cout << "offers = " << offers << std::endl;
    //std::cout << "#2\t" << offers->size() << "\tptr = " << &offers << '\n';

//    for (auto offer: *offers) {  // probably slow
//        std::cout << "#1" << std::endl;
//        if (offer.first == id) {
//            offerid = offer;
//            return true;
//        }
//    }

    return false;
}





void Level2::print_level2_by_price() {
    print
    printf("bids:\n");
    for (const auto &offer: bids_) {
        for (int i = 0; i < offer.second.size(); i++) {
            printf("price: [%llu]; bid id: [%llu]; count: [%llu]\n", offer.first, offer.second[i].first, offer.second[i].second);
        }
    }
    printf("asks:\n");
    for (const auto &offer: asks_) {
        for (int i = 0; i < offer.second.size(); i++) {
            printf("price: [%llu]; bid id: [%llu]; count: [%llu]\n", offer.first, offer.second[i].first, offer.second[i].second);
        }
    }
}

void Level2::print_level2_by_idx() {
    printf("print by indexes:\n");
    for (const auto& id: ids) {
        std::pair<OfferID, Count> offer;
        get_offer_by_id(id.first, offer);
        printf("id: [%llu], price: [%llu], count: [%llu]\n", id.first, id.second, offer.second);
    }
}

