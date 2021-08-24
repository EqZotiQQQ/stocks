#include "level2BinaryTreeBase.h"

#include <iostream>

Level2BinaryTreeBase::Level2BinaryTreeBase() {

}
auto Level2BinaryTreeBase::add_order(int quantity, Price price, bool isBid) -> OfferID {
    if (isBid) {    // выставляем на продажу
        while (price <= asks_.begin()->first && !asks_.empty() && quantity) {
            exchange_existing_offers(asks_, quantity);
        }
        if (quantity) {     //  Если остались элементы, то добавляем их в аски
            add_offer_to(bids_, price, quantity);
        }
    } else { // выставляем на покупку
        while (price >= bids_.begin()->first && !bids_.empty() && quantity) {
            exchange_existing_offers(bids_, quantity);
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
}

void Level2BinaryTreeBase::exchange_existing_offers(std::map<Price, vector<pair<OfferID, Count>>>& offer,
                                                    int& quantity) {
    int r = offer.begin()->second.begin()->second - quantity;
    if (r > 0) {            // Количество удаляемых акций меньше, чем мапа
        offer.begin()->second.begin()->second -= quantity;
        quantity = 0;
    } else if (r < 0) {     // Кол-во удаляемых больше, чем в мапе
        quantity -= offer.begin()->second.begin()->second;
        offer.begin()->second.erase(offer.begin()->second.begin());
    } else {                // столько же сколько в мапе
        offer.begin()->second.erase(offer.begin()->second.begin());
        quantity = 0;
    }
    if (offer.begin()->second.empty()) {    // Если для цены нет больше офферов - убрать бид
        offer.erase(offer.begin());
    }

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