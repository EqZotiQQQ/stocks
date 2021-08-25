#include "level2BinaryTreeBase.h"

#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iomanip>

using Price = uint64_t;     // price in cents but i guess i need to switch to something bigger to remove overflow issue
using OfferID = uint64_t;   // id of offer. It
using Count = uint64_t;
using Callback = std::function<void()>;

Level2BinaryTreeBase::Level2BinaryTreeBase() {

}

OfferID Level2BinaryTreeBase::add_order(int quantity, Price price, bool isBid) {
    if (isBid) {    // выставляем на продажу
        while (price <= asks_.begin()->first && !asks_.empty() && quantity) {
            exchange_existing_offers(asks_, asks_by_offer_, quantity);
        }
        if (quantity) {     //  Если остались элементы, то добавляем их в аски
            add_offer_to(bids_, bids_by_offer_, price, quantity, offer_id);
        }
    } else { // выставляем на покупку
        while (price >= bids_.begin()->first && !bids_.empty() && quantity) {
            exchange_existing_offers(bids_, bids_by_offer_, quantity);
        }
        if (quantity) {     //  Если остались элементы, то добавляем их в аски
            add_offer_to(asks_, asks_by_offer_, price, quantity, offer_id);
        }
    }
    return ++offer_id;
}

void Level2BinaryTreeBase::add_offer_to(std::map<Price, vector<pair<OfferID, Count>>>& offer,
                                        std::map<OfferID, pair<Price, Count>>& offer_by_id,
                                        Price price,
                                        int quantity,
                                        int id) {
    auto order = offer.find(price);
    if (order != offer.end()) {
        offer.insert({price, {}});
    }
    offer[price].push_back({id, quantity});
    offer_by_id.insert({id, {price, quantity}});
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

bool Level2BinaryTreeBase::close_order_support(std::map<Price, vector<pair<OfferID, Count>>>& bid_ask_orders,
                                               std::map<OfferID, pair<Price, Count>>& offer_by_id,
                                               int quantity,
                                               int id) {
    auto offer = offer_by_id.find(id);
    if (offer != offer_by_id.end()) {
        if (quantity <= offer->second.second) {
            offer_by_id[id].second -= quantity;

            for (int i = 0; i < bid_ask_orders[offer_by_id[id].first].size(); i++) {
                if (bid_ask_orders[offer_by_id[id].first].at(i).first == id) {
                    bid_ask_orders[offer_by_id[id].first].at(i).second -= quantity;
                    if (bid_ask_orders[offer_by_id[id].first].at(i).second == 0) {
                        bid_ask_orders[offer_by_id[id].first].erase(bid_ask_orders[offer_by_id[id].first].begin() + i);
                    }
                }
            }

            if (offer_by_id[id].second == 0) {
                offer_by_id.erase(id);
            }

            return true;
        } else {
            printf("Incorrect quantity for close order: [%lu] while [%lu] available.\n", quantity, offer->second.second);
        }
    }
    return false;
}

bool Level2BinaryTreeBase::close_order(unsigned int quantity, OfferID id) {
    return Level2BinaryTreeBase::close_order_support(bids_, bids_by_offer_, quantity, id)
        || Level2BinaryTreeBase::close_order_support(asks_, asks_by_offer_, quantity, id);
}

auto Level2BinaryTreeBase::get_offers_by_price(Price price) -> std::vector<std::pair<OfferID, Count>> {
    auto bid_offer = bids_.find(price);
    auto ask_offer = asks_.find(price);
    std::vector<std::pair<OfferID, Count>> ret {};
    if (bid_offer != bids_.end()) {
        ret.assign(bid_offer->second.begin(), bid_offer->second.end());
    } else if (ask_offer != asks_.end()) {
        ret.assign(ask_offer->second.begin(), ask_offer->second.end());
    } else {
        printf("No offers for [%lu] price\n", price);
    }
    return ret;
}

auto Level2BinaryTreeBase::get_offers_by_id(OfferID id) -> std::pair<Price, Count> {
    auto bid_offer = bids_by_offer_.find(id)->second;
    auto ask_offer = asks_by_offer_.find(id)->second;
    std::pair<Price, Count> ret {};
    if (bid_offer != bids_by_offer_.end()->second) {
        ret.first = bid_offer.first;
        ret.second = bid_offer.second;
    } else if (ask_offer != asks_by_offer_.end()->second) {
        ret.first = ask_offer.first;
        ret.second = ask_offer.second;
    } else {
        printf("No offer for [%llu] id\n", offer_id);
    }
    return ret;
}

void Level2BinaryTreeBase::print_level2_by_price() {
    printf("==============\nBids:\n");
    for (auto bid = bids_.rbegin(); bid != bids_.rend(); bid++) {
        for (const auto& offer: bid->second) {
            printf("Price [%llu]; ID: [%llu]; Count: [%llu]\n", bid->first, offer.first, offer.second);
        }
    }
    printf("==============\nAsks:\n");
    for (auto ask = asks_.rbegin(); ask != asks_.rend(); ask++) {
        for (const auto& offer: ask->second) {
            printf("Price [%llu]; ID: [%llu]; Count: [%llu]\n", ask->first, offer.first, offer.second);
        }
    }
}

void Level2BinaryTreeBase::print_level2_by_idx() {
    printf("==============\nBids:\n");
    for (const auto& bid: bids_by_offer_) {
        printf("OfferID: [%llu]; Price: [%llu]; Count: [%llu]\n",bid.first, bid.second.first, bid.second.second);
    }
    printf("==============\nAsks:\n");
    for (const auto& ask: asks_by_offer_) {
        printf("OfferID: [%llu]; Price: [%llu]; Count: [%llu]\n",ask.first, ask.second.first, ask.second.second);
    }
}

Count Level2BinaryTreeBase::get_l2_size() {
    Count l2_size {};
    for (const auto& i: bids_by_offer_) {
        l2_size += i.second.second;
    }
    for (const auto& i: asks_by_offer_) {
        l2_size += i.second.second;
    }
    return l2_size;
}

bool Level2BinaryTreeBase::store() {
    std::ofstream file("stocks.json");
    if (!file.is_open()) {
        return false;
    }
    nlohmann::json json;
    for (auto iter = bids_.begin(); iter != bids_.end(); iter++) {
        for (const auto& i: iter->second) {
            nlohmann::json offers_by_price;
            offers_by_price["bids"]["id"] = i.first;
            offers_by_price["bids"]["price"] = iter->first;
            offers_by_price["bids"]["quantity"] = i.second;
            json += offers_by_price;
        }
    }
    for (auto iter = asks_.begin(); iter != asks_.end(); iter++) {
        for (const auto& i: iter->second) {
            nlohmann::json offers_by_price;
            offers_by_price["asks"]["id"] = i.first;
            offers_by_price["asks"]["price"] = iter->first;
            offers_by_price["asks"]["quantity"] = i.second;
            json += offers_by_price;
        }
    }
    file << std::setw(4) << json << std::endl;
    return true;
}

bool Level2BinaryTreeBase::load() {
    std::ifstream file("stocks.json");
    if (!file.is_open()) {
        printf("Unable to open file\n");
        return false;
    }
    nlohmann::json json;
    file >> json;
    create_offer_structure(json);
    return true;
}

void Level2BinaryTreeBase::create_offer_structure(const nlohmann::basic_json<>& json) {
    for (const auto& j: json) {
        for (const auto& i: j) {
            if (j.begin().key() == "bids") {
                add_offer_to(bids_, bids_by_offer_, i["price"], i["quantity"], i["id"]);
            } else {
                add_offer_to(asks_, asks_by_offer_, i["price"], i["quantity"], i["id"]);
            }
        }
    }
}
