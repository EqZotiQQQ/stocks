#include "level2BinaryTreeBase.h"

#include <iostream>
#include <fstream>
#include <iomanip>

namespace binary_tree_base {

OfferID Level2BinaryTreeBase::add_order(Count quantity, Price price, OFFER offer_type) noexcept {

    if (offer_type == OFFER::BID) {    // выставляем на продажу
        Count bids_to_be_added = exchange_existing_offers(offer_type, price, quantity);  // Нужно обменять аски, если наш бид ниже по цене
        add_offer_to(bids_ordered_by_price_, bids_ordered_by_offer_, price, bids_to_be_added, offer_id_);
    } else { // выставляем на покупку
        Count asks_to_be_added = exchange_existing_offers(offer_type, price, quantity);
        add_offer_to(asks_ordered_by_price_, asks_ordered_by_offer_, price, asks_to_be_added, offer_id_);
    }
    return offer_id_++;
}

Count Level2BinaryTreeBase::exchange_existing_offers(OFFER offer_type, Price price, Count quantity) noexcept {

    std::map<Price, std::vector<OfferById>>* offers_ordered_by_price = nullptr;
    std::map<OfferID, OfferByPrice>* offers_ordered_by_id = nullptr;
    set_offers_by_type(offer_type, offers_ordered_by_price, offers_ordered_by_id);
    if (!offers_ordered_by_price || !offers_ordered_by_id) return quantity;

    auto first_offer_for_price = offers_ordered_by_price->begin();

    while (!offers_ordered_by_price->empty() && compare_prices(offer_type, price, first_offer_for_price->first) && quantity) {

        Count* quantity_for_offer = &first_offer_for_price->second.front().quantity; // cannot be null, checked in switch-case

        std::vector<OfferById>* offers_for_price = nullptr;
        if (!get_offers_by_price(first_offer_for_price->first, offers_for_price)) {
            break;
        }

        OfferByPrice* offerByPrice = nullptr;
        if (!get_offers_by_id(offers_for_price->front().offerId, offerByPrice)) {
            break;
        }

        OfferID id_key_to_be_removed = offers_for_price->front().offerId;
        Price price_key_to_be_removed = offerByPrice->price;

        if (*quantity_for_offer < quantity) {
            // Сработает если число акций в оффере больше чем есть в текущий момент в стакане ниже его
            quantity -= *quantity_for_offer;
            offerByPrice->quantity = 0;
            offers_for_price->front().quantity = 0;
        } else {
            if (*quantity_for_offer == quantity) {
                // Сработает если число акций в оффере столько же, сколько есть в текущий момент в стакане
                offerByPrice->quantity = 0;
                offers_for_price->front().quantity = 0;
            } else {
                // Сработает если число акций в оффере меньше, чем количество акций в текущий момент в стакане
                // overflow protected by __builtin_usubll_overflow
                offerByPrice->quantity -= quantity;
                offers_for_price->front().quantity -= quantity;
            }
            quantity = 0;
        }

        if (offerByPrice->quantity == 0) {
            offers_ordered_by_id->erase(id_key_to_be_removed);
            std::vector<OfferById> s = offers_ordered_by_price->find(price_key_to_be_removed)->second;
            for (int i = 0; i < s.size(); i++) {
                if (s[i].offerId == id_key_to_be_removed) {
                    s.erase(s.begin() + i);
                    break;
                }
            }
            if (s.empty()) {
                offers_ordered_by_price->erase(price_key_to_be_removed);
            }
        }

        first_offer_for_price = offers_ordered_by_price->begin();
    }
    return quantity;
}

// TODO: prob need to use set_offers_by_type to reduce number of args
void Level2BinaryTreeBase::add_offer_to(std::map<Price, std::vector<OfferById>>& offers_ordered_by_price,
                                        std::map<OfferID, OfferByPrice>& offers_ordered_by_id,
                                        Price price,
                                        Count quantity,
                                        OfferID id) noexcept {
    if (quantity) {
        auto order = offers_ordered_by_price.find(price);
        if (order != offers_ordered_by_price.end()) {
            offers_ordered_by_price.insert({price, {}});
        }
        offers_ordered_by_price[price].push_back({id, quantity});
        offers_ordered_by_id.insert({id, {price, quantity}});
    }
}

bool Level2BinaryTreeBase::get_offers_by_price(Price price, std::vector<OfferById>*& v) noexcept {
    auto bid_offer = bids_ordered_by_price_.find(price);
    auto ask_offer = asks_ordered_by_price_.find(price);
    if (bid_offer != bids_ordered_by_price_.end()) {
        v = &bids_ordered_by_price_.at(price);
    } else if (ask_offer != asks_ordered_by_price_.end()) {
        v = &asks_ordered_by_price_.at(price);
    } else {
        printf("No offers for [%lu] price\n", price);
        return false;
    }
    return true;
}

auto Level2BinaryTreeBase::get_offers_by_price(Price price) const noexcept -> std::vector<OfferById> {
    auto bid_offer = bids_ordered_by_price_.find(price);
    auto ask_offer = asks_ordered_by_price_.find(price);
    std::vector<OfferById> ret;
    if (bid_offer != bids_ordered_by_price_.end()) {
        ret.assign(bid_offer->second.begin(), bid_offer->second.end());
    } else if (ask_offer != asks_ordered_by_price_.end()) {
        ret.assign(ask_offer->second.begin(), ask_offer->second.end());
    } else {
        printf("No offers for [%lu] price\n", price);
    }
    return ret;
}

bool Level2BinaryTreeBase::get_offers_by_id(OfferID id, OfferByPrice*& v) noexcept {
    if (bids_ordered_by_offer_.find(id) != bids_ordered_by_offer_.end()) {
        v = &bids_ordered_by_offer_.at(id);
    } else if (asks_ordered_by_offer_.find(id) != asks_ordered_by_offer_.end()) {
        v = &asks_ordered_by_offer_.at(id);
    } else {
        printf("No offer for [%llu] id\n", offer_id_);
        return false;
    }
    return true;
}

auto Level2BinaryTreeBase::get_offers_by_id(OfferID id) const noexcept -> OfferByPrice {
    OfferByPrice ret {0,0};
    if (bids_ordered_by_offer_.find(id) != bids_ordered_by_offer_.end()) {
        ret.price =  bids_ordered_by_offer_.at(id).price;
        ret.quantity = bids_ordered_by_offer_.at(id).quantity;
    } else if (asks_ordered_by_offer_.find(id) != asks_ordered_by_offer_.end()) {
        ret.price = asks_ordered_by_offer_.at(id).price;
        ret.quantity = asks_ordered_by_offer_.at(id).quantity;
    } else {
        printf("No offer for [%llu] id\n", offer_id_);
    }
    return ret;
}

bool Level2BinaryTreeBase::close_order(Count quantity, OfferID id) noexcept {
    if (bids_ordered_by_offer_.find(id) != bids_ordered_by_offer_.end()) {
        return close_order_support(OFFER::ASK, quantity, id);
    } else if (asks_ordered_by_offer_.find(id) != asks_ordered_by_offer_.end()) {
        return close_order_support(OFFER::BID, quantity, id);
    } else {
        return false;
    }
}

bool Level2BinaryTreeBase::close_order_support(OFFER offer_type,
                                               Count quantity,
                                               OfferID id) noexcept {

    std::map<OfferID, OfferByPrice>* offers_ordered_by_id = nullptr;
    std::map<Price, std::vector<OfferById>>* offers_ordered_by_price = nullptr;

    set_offers_by_type(offer_type, offers_ordered_by_price, offers_ordered_by_id);

    if (!offers_ordered_by_price || !offers_ordered_by_id || offers_ordered_by_id->find(id) == offers_ordered_by_id->end())
        return false;

    Price offer_price = offers_ordered_by_id->at(id).price;  // to find in second map

    std::vector<OfferById>* ids_by_price = nullptr;
    if (!get_offers_by_price(offer_price, ids_by_price)) {
        return false;
    }

    if (offers_ordered_by_id->at(id).quantity < quantity) {
        // Сработает если число акций в оффере больше чем есть в текущий момент в стакане ниже его
        return false;
    } else {
        OfferID id_position = 0;
        for (; id_position < ids_by_price->size(); id_position++) {
            if (ids_by_price->at(id_position).offerId == id) {
                break;
            }
        }

        if (offers_ordered_by_id->at(id).quantity == quantity) {
            // Сработает если число акций в оффере столько же, сколько есть в текущий момент в стакане
            offers_ordered_by_id->erase(id);

            ids_by_price->erase(ids_by_price->begin() + id_position);
            if (ids_by_price->empty()) {
                offers_ordered_by_price->erase(offer_price);
            }
        } else {
            // Сработает если число акций в оффере меньше, чем количество акций в текущий момент в стакане
            offers_ordered_by_id->at(id).quantity -= quantity;
            ids_by_price->at(id_position).quantity -= quantity;
        }
    }
    return true;
}

void Level2BinaryTreeBase::print_level2_by_price() const noexcept {
    printf("==============\nBids:\n");
    for (auto bid = bids_ordered_by_price_.rbegin(); bid != bids_ordered_by_price_.rend(); bid++) {
        for (const auto& offer: bid->second) {
            printf("Price [%llu]; ID: [%llu]; Count: [%llu]\n", bid->first, offer.offerId, offer.quantity);
        }
    }
    printf("==============\nAsks:\n");
    for (auto ask = asks_ordered_by_price_.rbegin(); ask != asks_ordered_by_price_.rend(); ask++) {
        for (const auto& offer: ask->second) {
            printf("Price [%llu]; ID: [%llu]; Count: [%llu]\n", ask->first, offer.offerId, offer.quantity);
        }
    }
}

void Level2BinaryTreeBase::print_level2_by_idx() const noexcept {
    printf("==============\nBids:\n");
    for (const auto& bid: bids_ordered_by_offer_) {
        printf("OfferID: [%llu]; Price: [%llu]; Count: [%llu]\n",bid.first, bid.second.price, bid.second.quantity);
    }
    printf("==============\nAsks:\n");
    for (const auto& ask: asks_ordered_by_offer_) {
        printf("OfferID: [%llu]; Price: [%llu]; Count: [%llu]\n",ask.first, ask.second.price, ask.second.quantity);
    }
}

bool Level2BinaryTreeBase::compare_prices(OFFER offer_type, Price minimum_bid_price, Price maximum_ask_price) {
    if (offer_type == OFFER::BID) {
        return minimum_bid_price <= maximum_ask_price;
    } else {
        return minimum_bid_price >= maximum_ask_price;
    }
}

void Level2BinaryTreeBase::set_offers_by_type(OFFER offer_type,
                                              std::map<Price, std::vector<OfferById>>*& offer_by_price,
                                              std::map<OfferID, OfferByPrice>*& offer_by_id) {
    if (offer_type == OFFER::BID) {
        offer_by_price = &asks_ordered_by_price_;
        offer_by_id = &asks_ordered_by_offer_;
    } else {
        offer_by_price = &bids_ordered_by_price_;
        offer_by_id = &bids_ordered_by_offer_;
    }
}

Count Level2BinaryTreeBase::get_l2_size() const noexcept {
    Count l2_size {};
    for (const auto& i: bids_ordered_by_offer_) {
        l2_size += i.second.quantity;
    }
    for (const auto& i: asks_ordered_by_offer_) {
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
    for (auto iter = bids_ordered_by_price_.rbegin(); iter != bids_ordered_by_price_.rend(); iter++) {
        for (const auto& i: iter->second) {
            nlohmann::json offers_by_price;
            offers_by_price["bids"]["id"] = i.offerId;
            offers_by_price["bids"]["price"] = iter->first;
            offers_by_price["bids"]["quantity"] = i.quantity;
            json += offers_by_price;
        }
    }
    for (auto iter = asks_ordered_by_price_.rbegin(); iter != asks_ordered_by_price_.rend(); iter++) {
        for (const auto& i: iter->second) {
            nlohmann::json offers_by_price;
            offers_by_price["asks"]["id"] = i.offerId;
            offers_by_price["asks"]["price"] = iter->first;
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
                add_offer_to(bids_ordered_by_price_, bids_ordered_by_offer_, m["price"], m["quantity"], m["id"]);
            } else {
                add_offer_to(asks_ordered_by_price_, asks_ordered_by_offer_, m["price"], m["quantity"], m["id"]);
            }
        }
    }
}

}
