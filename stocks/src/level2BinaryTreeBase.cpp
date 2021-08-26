#include "level2BinaryTreeBase.h"

#include <iostream>
#include <fstream>
#include <iomanip>

OfferID Level2BinaryTreeBase::add_order(Count quantity, Price price, OFFER offer_type) noexcept {

    if (offer_type == OFFER::BID) {    // выставляем на продажу
        exchange_existing_offers(offer_type, price, quantity);  // Нужно обменять аски, если наш бид ниже по цене
        if (quantity) {     //  Если остались элементы, то добавляем их в биды
            add_offer_to(bids_by_price_, bids_by_offer_, price, quantity, offer_id);
        }
    } else { // выставляем на покупку
        exchange_existing_offers(offer_type, price, quantity); // наоборот :10
        if (quantity) {     //  Если остались элементы, то добавляем их в аски
            add_offer_to(asks_by_price_, asks_by_offer_, price, quantity, offer_id);
        }
    }
    return ++offer_id;
}

void Level2BinaryTreeBase::exchange_existing_offers(OFFER offer_type, Price price, Count& quantity) noexcept {

    std::map<Price, vector<OfferById>>* offer_by_price = nullptr;
    std::map<OfferID, OfferByPrice>* offer_by_id = nullptr;

    set_offers_by_type(offer_type, offer_by_price, offer_by_id);

    if (!offer_by_price || !offer_by_id) return;

    while (!offer_by_price->empty() && compare_prices(offer_type, price, offer_by_price->begin()->first) && quantity) {
        Count count_diff {};

        Count* quantity_for_offer = &offer_by_price->begin()->second.front().quantity; // cannot be null, checked in switch-case

        std::vector<OfferById>* v = nullptr;
        if (!get_offers_by_price(offer_by_price->begin()->first, v)) {
            return;
        }

        OfferByPrice* p = nullptr;
        if (!get_offers_by_id(v->front().offerId, p)) {
            return;
        }

        OfferID id_key_to_be_removed = v->front().offerId;
        Price price_key_to_be_removed = p->price;

        if (__builtin_usubll_overflow(*quantity_for_offer, quantity, &count_diff)) {
            // Сработает если число акций в оффере больше чем есть в текущий момент в стакане ниже его
            quantity -= *quantity_for_offer;
            p->quantity = 0;
            v->front().quantity = 0;
        } else {
            if (*quantity_for_offer == quantity) {
                // Сработает если число акций в оффере столько же, сколько есть в текущий момент в стакане
                p->quantity = 0;
                v->front().quantity = 0;
            } else {
                // Сработает если число акций в оффере меньше, чем количество акций в текущий момент в стакане
                // overflow protected by __builtin_usubll_overflow
                p->quantity -= quantity;
                v->front().quantity -= quantity;
            }
            quantity = 0;
        }

        if (p->quantity == 0) {
            offer_by_id->erase(id_key_to_be_removed);
            std::vector<OfferById> s = offer_by_price->find(price_key_to_be_removed)->second;
            for (int i = 0; i < s.size(); i++) {
                if (s[i].offerId == id_key_to_be_removed) {
                    s.erase(s.begin() + i);
                    break;
                }
            }
            if (s.empty()) {
                offer_by_price->erase(price_key_to_be_removed);
            }
        }
    }
}

// TODO: prob need to use set_offers_by_type to reduce number of args
void Level2BinaryTreeBase::add_offer_to(std::map<Price, vector<OfferById>>& offer,
                                        std::map<OfferID, OfferByPrice>& offer_by_id,
                                        Price price,
                                        Count quantity,
                                        OfferID id) noexcept {

    auto order = offer.find(price);
    if (order != offer.end()) {
        offer.insert({price, {}});
    }
    offer[price].push_back({id, quantity});
    offer_by_id.insert({id, {price, quantity}});
}

bool Level2BinaryTreeBase::get_offers_by_price(Price price, std::vector<OfferById>*& v) noexcept {
    auto bid_offer = bids_by_price_.find(price);
    auto ask_offer = asks_by_price_.find(price);
    if (bid_offer != bids_by_price_.end()) {
        v = &bids_by_price_.at(price);
    } else if (ask_offer != asks_by_price_.end()) {
        v = &asks_by_price_.at(price);
    } else {
        printf("No offers for [%lu] price\n", price);
        return false;
    }
    return true;
}

auto Level2BinaryTreeBase::get_offers_by_price(Price price) const noexcept -> std::vector<OfferById> {
    auto bid_offer = bids_by_price_.find(price);
    auto ask_offer = asks_by_price_.find(price);
    std::vector<OfferById> ret;
    if (bid_offer != bids_by_price_.end()) {
        ret.assign(bid_offer->second.begin(), bid_offer->second.end());
    } else if (ask_offer != asks_by_price_.end()) {
        ret.assign(ask_offer->second.begin(), ask_offer->second.end());
    } else {
        printf("No offers for [%lu] price\n", price);
    }
    return ret;
}

bool Level2BinaryTreeBase::get_offers_by_id(OfferID id, OfferByPrice*& v) noexcept {
    if (bids_by_offer_.find(id) != bids_by_offer_.end()) {
        v = &bids_by_offer_.at(id);
    } else if (asks_by_offer_.find(id) != asks_by_offer_.end()) {
        v = &asks_by_offer_.at(id);
    } else {
        printf("No offer for [%llu] id\n", offer_id);
        return false;
    }
    return true;
}

auto Level2BinaryTreeBase::get_offers_by_id(OfferID id) const noexcept -> OfferByPrice {
    OfferByPrice ret {0,0};
    if (bids_by_offer_.find(id) != bids_by_offer_.end()) {
        ret.price =  bids_by_offer_.at(id).price;
        ret.quantity = bids_by_offer_.at(id).quantity;
    } else if (asks_by_offer_.find(id) != asks_by_offer_.end()) {
        ret.price = asks_by_offer_.at(id).price;
        ret.quantity = asks_by_offer_.at(id).quantity;
    } else {
        printf("No offer for [%llu] id\n", offer_id);
    }
    return ret;
}

bool Level2BinaryTreeBase::close_order(Count quantity, OfferID id) noexcept {
    if (bids_by_offer_.find(id) != bids_by_offer_.end()) {
        return close_order_support(OFFER::ASK, quantity, id);
    } else if (asks_by_offer_.find(id) != asks_by_offer_.end()) {
        return close_order_support(OFFER::BID, quantity, id);
    } else {
        return false;
    }
}

bool Level2BinaryTreeBase::close_order_support(OFFER offer_type,
                                               Count quantity,
                                               OfferID id) noexcept {

    std::map<OfferID, OfferByPrice>* offer_by_id = nullptr;
    std::map<Price, vector<OfferById>>* offer_by_price = nullptr;

    set_offers_by_type(offer_type, offer_by_price, offer_by_id);

    if (!offer_by_price || !offer_by_id || offer_by_id->find(id) == offer_by_id->end())
        return false;

    Price exist_close_diff {};
    Price offer_price = offer_by_id->at(id).price;  // to find in second map

    vector<OfferById>* ids_by_price = nullptr;
    if (!get_offers_by_price(offer_price, ids_by_price)) {
        return false;
    }

    if (offer_by_id->at(id).quantity < quantity) {
        // Сработает если число акций в оффере больше чем есть в текущий момент в стакане ниже его
        return false;
    } else {

        OfferID id_position = 0;
        for (; id_position < ids_by_price->size(); id_position++) {
            if (ids_by_price->at(id_position).offerId == id) {
                break;
            }
        }

        if (offer_by_id->at(id).quantity == quantity) {
            // Сработает если число акций в оффере столько же, сколько есть в текущий момент в стакане
            offer_by_id->erase(id);

            ids_by_price->erase(ids_by_price->begin() + id_position);
            if (ids_by_price->empty()) {
                offer_by_price->erase(offer_price);
            }

        } else {
            // Сработает если число акций в оффере меньше, чем количество акций в текущий момент в стакане
            offer_by_id->at(id).quantity -= quantity;
            ids_by_price->at(id_position).quantity -= quantity;
        }
    }

    return true;
}

void Level2BinaryTreeBase::print_level2_by_price() const noexcept {
    printf("==============\nBids:\n");
    for (auto bid = bids_by_price_.rbegin(); bid != bids_by_price_.rend(); bid++) {
        for (const auto& offer: bid->second) {
            printf("Price [%llu]; ID: [%llu]; Count: [%llu]\n", bid->first, offer.offerId, offer.quantity);
        }
    }
    printf("==============\nAsks:\n");
    for (auto ask = asks_by_price_.rbegin(); ask != asks_by_price_.rend(); ask++) {
        for (const auto& offer: ask->second) {
            printf("Price [%llu]; ID: [%llu]; Count: [%llu]\n", ask->first, offer.offerId, offer.quantity);
        }
    }
}

void Level2BinaryTreeBase::print_level2_by_idx() const noexcept {
    printf("==============\nBids:\n");
    for (const auto& bid: bids_by_offer_) {
        printf("OfferID: [%llu]; Price: [%llu]; Count: [%llu]\n",bid.first, bid.second.price, bid.second.quantity);
    }
    printf("==============\nAsks:\n");
    for (const auto& ask: asks_by_offer_) {
        printf("OfferID: [%llu]; Price: [%llu]; Count: [%llu]\n",ask.first, ask.second.price, ask.second.quantity);
    }
}

bool Level2BinaryTreeBase::compare_prices(OFFER offer_type, Price rhs, Price lhs) {
    if (offer_type == OFFER::BID) {
        return rhs <= lhs;
    } else {
        return rhs >= lhs;
    }
}

void Level2BinaryTreeBase::set_offers_by_type(OFFER offer_type,
                                              std::map<Price, vector<OfferById>>*& offer_by_price,
                                              std::map<OfferID, OfferByPrice>*& offer_by_id) {
    if (offer_type == OFFER::BID) {
        offer_by_price = &asks_by_price_;
        offer_by_id = &asks_by_offer_;
    } else {
        offer_by_price = &bids_by_price_;
        offer_by_id = &bids_by_offer_;
    }
}

Count Level2BinaryTreeBase::get_l2_size() const noexcept {
    Count l2_size {};
    for (const auto& i: bids_by_offer_) {
        l2_size += i.second.quantity;
    }
    for (const auto& i: asks_by_offer_) {
        l2_size += i.second.quantity;
    }
    return l2_size;
}

bool Level2BinaryTreeBase::store() const noexcept {
    std::ofstream file("stocks.json");
    if (!file.is_open()) {
        return false;
    }
    nlohmann::json json;
    for (const auto & iter : bids_by_price_) {
        for (const auto& i: iter.second) {
            nlohmann::json offers_by_price;
            offers_by_price["bids"]["id"] = i.offerId;
            offers_by_price["bids"]["price"] = iter.first;
            offers_by_price["bids"]["quantity"] = i.quantity;
            json += offers_by_price;
        }
    }
    for (const auto& iter : asks_by_price_) {
        for (const auto& i: iter.second) {
            nlohmann::json offers_by_price;
            offers_by_price["asks"]["id"] = i.offerId;
            offers_by_price["asks"]["price"] = iter.first;
            offers_by_price["asks"]["quantity"] = i.quantity;
            json += offers_by_price;
        }
    }
    file << std::setw(4) << json << std::endl;
    return true;
}

bool Level2BinaryTreeBase::load() noexcept {
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

void Level2BinaryTreeBase::create_offer_structure(const nlohmann::basic_json<>& json) noexcept {
    for (const auto& str: json) {
        for (const auto& m: str) {
            if (str.begin().key() == "bids") {
                add_offer_to(bids_by_price_, bids_by_offer_, m["price"], m["quantity"], m["id"]);
            } else {
                add_offer_to(asks_by_price_, asks_by_offer_, m["price"], m["quantity"], m["id"]);
            }
        }
    }
}
