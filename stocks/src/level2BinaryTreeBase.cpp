#include "level2BinaryTreeBase.h"

#include <iostream>

Level2BinaryTreeBase::Level2BinaryTreeBase() {

}
auto Level2BinaryTreeBase::add_order(int quantity, Price price, bool isBid) -> OfferID {
    if (isBid) {    // выставляем на продажу
        while (price <= asks_.begin()->first && !asks_.empty() && quantity) {
            int r = asks_.begin()->second.begin()->second - quantity;
            if (r > 0) {            // Количество добавляемых акций больше, чем мапа на продажу
                asks_.begin()->second.begin()->second -= quantity;
                std::size_t id = asks_.begin()->second.begin()->first;  // Сохранить id для передачи его в sort by idx
                asks_by_offer_[id].second -= quantity;
                quantity = 0;
            } else if (r < 0) {     // Кол-во удаляемых меньше, чем в мапе на продажу
                quantity -= asks_.begin()->second.begin()->second;
                asks_.begin()->second.erase(asks_.begin()->second.begin());
            } else {                // столько же сколько в мапе на продажу
                asks_.begin()->second.erase(asks_.begin()->second.begin());
                std::size_t id = asks_.begin()->second.begin()->first;  // Сохранить id для передачи его в sort by idx
                asks_by_offer_[id].second -= quantity;
                quantity = 0;
            }
            if (asks_.begin()->second.empty()) {    // Если для цены нет больше офферов - убрать бид
                asks_.erase(asks_.begin());
            }
        }
        if (quantity) {     //  Если остались элементы, то добавляем их в аски
            add_offer_to(bids_, price, quantity);
        }
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
            add_offer_to(asks_, price, quantity);
        }
    }
    return ++offer_id;
}

void Level2BinaryTreeBase::add_offer_to(std::map<Price, vector<pair<OfferID, Count>>>& offer,
                  Price price,
                  int quantity) {
    auto order = offer.find(price);
    if (order != offer.end()) {
        offer.insert({price, {}});
    }
    offer[price].push_back({offer_id, quantity});
    asks_by_offer_[offer_id] = {price, quantity};
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
    printf("==============\nAsks:\n");
    for (auto ask = asks_.rbegin(); ask != asks_.rend(); ask++) {
        for (auto offer: ask->second) {
            printf("Price [%llu]; ID: [%llu]; Count: [%llu]\n", ask->first, offer.first, offer.second);
        }
    }
}
void Level2BinaryTreeBase::print_level2_by_idx() {
    printf("==============\nBids:\n");
    for (auto bid: bids_by_offer_) {
        printf("OfferID: [%llu]; Price: [%llu]; Count: [%llu]\n",bid.first, bid.second.first, bid.second.second);
    }
    printf("==============\nAsks:\n");
    for (auto ask: asks_by_offer_) {
        printf("OfferID: [%llu]; Price: [%llu]; Count: [%llu]\n",ask.first, ask.second.first, ask.second.second);
    }
}
auto Level2BinaryTreeBase::get_l2_size() -> OfferID {
    return 42;
}