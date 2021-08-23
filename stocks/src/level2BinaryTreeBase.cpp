#include "level2BinaryTreeBase.h"

#include <iostream>

Level2BinaryTreeBase::Level2BinaryTreeBase() {

}
auto Level2BinaryTreeBase::add_order(int quantity, Price price, bool isBid) -> OfferID {
    if (isBid) {    // выставляем на продажу
        while (price <= asks_.begin()->first && !asks_.empty() && quantity) {
            std::cout << "??\n";
            break;
        }
        auto order = bids_.find(price);
        if (order != bids_.end()) {
            bids_.insert({price, {}});
        }
        bids_[price].push_back({offer_id, quantity});
    } else { // выставляем на покупку
        while (price >= bids_.begin()->first && !bids_.empty() && quantity) {
            int r = bids_.begin()->second.begin()->second - quantity;
            if (r > 0) {            // Количество удаляемых акций меньше, чем мапа
                bids_.begin()->second.begin()->second -= quantity;
                quantity = 0;
            } else if (r < 0) {     // Кол-во удаляемых больше, чем в мапе
                quantity -= bids_.begin()->second.begin()->second;
                bids_.begin()->second.erase(bids_.begin()->second.begin());
            } else {                // столько же сколько в мапе
                bids_.begin()->second.erase(bids_.begin()->second.begin());
                quantity = 0;
            }
            if (bids_.begin()->second.empty()) {    // Если для цены нет больше офферов - убрать бид
                bids_.erase(bids_.begin());
            }
        }
        if (quantity) {     //  Если остались элементы, то добавляем их в аски
            auto order = asks_.find(price);
            if (order != asks_.end()) {
                asks_.insert({price, {}});
            }
            asks_[price].push_back({offer_id, quantity});
        }
    }
    return ++offer_id;
}
bool Level2BinaryTreeBase::close_order(unsigned int quantity, OfferID id) {
    return true;
}
bool Level2BinaryTreeBase::get_offers_by_price(Price price, std::vector<std::pair<OfferID, Count>>*& vec) {
    return true;
}
bool Level2BinaryTreeBase::get_offers_by_id(OfferID id, std::pair<OfferID, Count>*& offer_id) {
    return true;
}
void Level2BinaryTreeBase::print_level2_by_price() {
    printf("==============\nBids:\n");
    for (auto bid = bids_.rbegin(); bid != bids_.rend(); bid++) {
        for (auto offer: bid->second) {
            printf("Price [%llu]; ID: [%llu]; Count: [%llu]\n", bid->first, offer.first, offer.second);
        }
    }
//    for (auto bid: bids_) {
//        for (auto offer: bid.second) {
//            printf("Price [%llu]; ID: [%llu]; Count: [%llu]\n", bid.first, offer.first, offer.second);
//        }
//    }
    printf("==============\nAsks:\n");
    for (auto ask = asks_.rbegin(); ask != asks_.rend(); ask++) {
        for (auto offer: ask->second) {
            printf("Price [%llu]; ID: [%llu]; Count: [%llu]\n", ask->first, offer.first, offer.second);
        }
    }
//    for (auto bid: asks_) {
//        for (auto offer: bid.second) {
//            printf("Price [%llu]; ID: [%llu]; Count: [%llu]\n", bid.first, offer.first, offer.second);
//        }
//    }
}
void Level2BinaryTreeBase::print_level2_by_idx() {

}
auto Level2BinaryTreeBase::get_l2_size() -> OfferID {
    return 42;
}