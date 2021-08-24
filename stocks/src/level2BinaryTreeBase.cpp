#include "level2BinaryTreeBase.h"

#include <iostream>
#include <numeric>

using Price = uint64_t;     // price in cents but i guess i need to switch to something bigger to remove overflow issue
using OfferID = uint64_t;   // id of offer. It
using Count = uint64_t;
using Callback = std::function<void()>;

Level2BinaryTreeBase::Level2BinaryTreeBase() {

}

auto Level2BinaryTreeBase::add_order(int quantity, Price price, bool isBid) -> OfferID {
    if (isBid) {    // выставляем на продажу
        while (price <= asks_.begin()->first && !asks_.empty() && quantity) {
            exchange_existing_offers(asks_, asks_by_offer_, quantity);
        }
        if (quantity) {     //  Если остались элементы, то добавляем их в аски
            add_offer_to(bids_, bids_by_offer_, price, quantity);
        }
    } else { // выставляем на покупку
        while (price >= bids_.begin()->first && !bids_.empty() && quantity) {
            exchange_existing_offers(bids_, bids_by_offer_, quantity);
        }
        if (quantity) {     //  Если остались элементы, то добавляем их в аски
            add_offer_to(asks_, asks_by_offer_, price, quantity);
        }
    }
    return ++offer_id;
}

void Level2BinaryTreeBase::add_offer_to(std::map<Price, vector<pair<OfferID, Count>>>& offer,
                                        std::map<OfferID, pair<Price, Count>>& offer_by_id,
                                        Price price,
                                        int quantity) {
    auto order = offer.find(price);
    if (order != offer.end()) {
        offer.insert({price, {}});
    }
    offer[price].push_back({offer_id, quantity});
    offer_by_id.insert({offer_id, {price, quantity}});
}

void Level2BinaryTreeBase::exchange_existing_offers(std::map<Price, vector<pair<OfferID, Count>>>& offer,
                                                    std::map<OfferID, pair<Price, Count>>& offer_by_id,
                                                    int& quantity) {
    int r = offer.begin()->second.begin()->second - quantity;
    if (r > 0) {            // Количество удаляемых акций меньше, чем мапа
        OfferID offers_id = offer_by_id.find(offer.begin()->second.begin()->first)->first;
        offer_by_id[offers_id].second -= quantity;

        offer.begin()->second.begin()->second -= quantity;
        quantity = 0;

    } else if (r < 0) {     // Кол-во удаляемых больше, чем в мапе
        OfferID offers_id = offer_by_id.find(offer.begin()->second.begin()->first)->first;
        offer_by_id.erase(offers_id);

        quantity -= offer.begin()->second.begin()->second;
        offer.begin()->second.erase(offer.begin()->second.begin());

    } else {                // столько же сколько в мапе
        OfferID offers_id = offer_by_id.find(offer.begin()->second.begin()->first)->first;
        offer_by_id.erase(offers_id);

        offer.begin()->second.erase(offer.begin()->second.begin());
        quantity = 0;
    }
    if (offer.begin()->second.empty()) {    // Если для цены нет больше офферов - убрать бид
        offer.erase(offer.begin());
    }
}

bool Level2BinaryTreeBase::close_order(unsigned int quantity, OfferID id) {
    auto bid_offer = bids_by_offer_.find(id);
    auto ask_offer = asks_by_offer_.find(id);
    if (bid_offer != bids_by_offer_.end()) {
        if (quantity <= bid_offer->second.second) {
            bids_by_offer_[id].second -= quantity;

            for (int i = 0; i < bids_[bids_by_offer_[id].first].size(); i++) {
                if (bids_[bids_by_offer_[id].first].at(i).first == id) {
                    bids_[bids_by_offer_[id].first].at(i).second -= quantity;
                    if (bids_[bids_by_offer_[id].first].at(i).second == 0) {
                        bids_[bids_by_offer_[id].first].erase(bids_[bids_by_offer_[id].first].begin() + i);
                    }
                }
            }

            if (bids_by_offer_[id].second == 0) {
                bids_by_offer_.erase(id);
            }

            return true;
        } else {
            printf("Incorrect quantity for close order: [%lu] while [%lu] available.\n", quantity, bid_offer->second.second);
        }
    } else if (ask_offer != asks_by_offer_.end()) {
        if (quantity <= ask_offer->second.second) {
            asks_by_offer_[id].second -= quantity;

            for (int i = 0; i < asks_[asks_by_offer_[id].first].size(); i++) {
                if (asks_[asks_by_offer_[id].first].at(i).first == id) {
                    asks_[asks_by_offer_[id].first].at(i).second -= quantity;
                    if (asks_[asks_by_offer_[id].first].at(i).second == 0) {
                        asks_[asks_by_offer_[id].first].erase(asks_[asks_by_offer_[id].first].begin() + i);
                    }
                }
            }

            if (asks_by_offer_[id].second == 0) {
                asks_by_offer_.erase(id);
            }
            return true;
        } else {
            printf("Incorrect quantity for close order: [%lu] while [%lu] available.\n", quantity, bid_offer->second.second);
        }
    } else {
        printf("No offer for [%lu] id\n", id);
    }
    return false;
}

bool Level2BinaryTreeBase::get_offers_by_price(Price price, std::vector<std::pair<OfferID, Count>>*& vec) {
    auto bid_offer = bids_.find(price);
    auto ask_offer = asks_.find(price);
    if (bid_offer != bids_.end()) {
        vec = &bid_offer->second;
    } else if (ask_offer != asks_.end()) {
        vec = &ask_offer->second;
    } else {
        printf("No offers for [%lu] price\n", price);
        return false;
    }
    return true;
}

bool Level2BinaryTreeBase::get_offers_by_id(OfferID id, std::pair<Price, Count>*& offer) {
    auto bid_offer = bids_by_offer_.find(id)->second;
    auto ask_offer = asks_by_offer_.find(id)->second;
    if (bid_offer != bids_by_offer_.end()->second) {
        offer = &bid_offer;
    } else if (ask_offer != asks_by_offer_.end()->second) {
        offer = &ask_offer;
    } else {
        printf("No offer for [%llu] id\n", offer_id);
        return false;
    }
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

Count Level2BinaryTreeBase::get_l2_size() {
    Count l2_size {};
    for (auto i: bids_by_offer_) {
        l2_size += i.second.second;
    }
    for (auto i: asks_by_offer_) {
        l2_size += i.second.second;
    }
    return l2_size;
}