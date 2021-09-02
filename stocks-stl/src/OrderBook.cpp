#include "OrderBook.h"

#include <fstream>
#include <iomanip>
#include <iostream>

namespace stl {

OfferID OrderBook::add_order(OFFER offer_type, Price price, Qty quantity) noexcept
{
    if (offer_type == OFFER::BID) {
        quantity = exchange_offers(asks_, price, quantity);
        add_offer_to(bids_, price, quantity, offer_id_);
    } else {
        quantity = exchange_offers(bids_, price, quantity);
        add_offer_to(asks_, price, quantity, offer_id_);
    }
    return offer_id_++;
}

void OrderBook::add_offer_to(Orders orders, Price price, Qty quantity, OfferID id) noexcept
{
    if (quantity != 0u) {

        if (!orders.unordered_price_.insert(price).second) {
            unordered_offer_id_.emplace(id);
        }
        orders.by_price_[price]++;
        unordered_offer_id_.emplace(id);
        orders.by_id_.insert(id);

        id_to_count_.emplace(id, quantity);
        id_to_price_.emplace(id, price);

        if (!price_to_id_.emplace(price, std::set<OfferID>{id}).second) {
            price_to_id_[price].insert(id);
        }
    }
}

Qty OrderBook::exchange_offers(Orders orders, Price offer_price, Qty offer_quantity) noexcept
{
    if (offer_quantity == 0 || orders.by_id_.empty()) {
        return offer_quantity;
    }

    Price most_expensive_order = asks_.by_price_.rbegin()->first;
    std::cout << "expens: " << most_expensive_order << std::endl;
    Price cheapest_order = bids_.by_price_.begin()->first;
    std::cout << "cheapest: " << cheapest_order << std::endl;

    if (&orders == &bids_) {
        if (most_expensive_order < offer_price) { /// red <= green - skip
            return offer_quantity;
        }
    } else {
        if (cheapest_order > offer_price) { /// red > green - skip
            return offer_quantity;
        }
    }
    OfferID id_to_remove = *price_to_id_[cheapest_order].begin();
    Qty existing_offer_count = id_to_count_[id_to_remove];

    if (existing_offer_count > offer_quantity) {
        __builtin_usubll_overflow(id_to_count_[id_to_remove], offer_quantity, &id_to_count_[id_to_remove]);
        offer_quantity = 0;
    } else {
        offer_quantity = close_order_helper(id_to_remove, orders, offer_quantity);
    }
    return exchange_offers(orders, offer_price, offer_quantity);
}

Qty OrderBook::close_order_helper(OfferID id, Orders orders, Qty offer_quantity)
{
    Price cheapest_order = orders.by_price_.begin()->first;

    orders.by_id_.erase(id);
    Qty remove = id_to_count_[id];
    __builtin_usubll_overflow(offer_quantity, remove, &offer_quantity);
    id_to_count_.erase(id);
    id_to_price_.erase(id);
    OfferID offers_for_price_left = --orders.by_price_[cheapest_order];
    unordered_offer_id_.erase(id);
    if (offers_for_price_left == 0) {
        orders.unordered_price_.erase(cheapest_order);
        price_to_id_.erase(cheapest_order);
        orders.by_price_.erase(cheapest_order);
    } else {
        auto iter = price_to_id_[cheapest_order].begin();
        price_to_id_[cheapest_order].erase(iter);
    }
    return offer_quantity;
}

bool OrderBook::close_order(OfferID id, Qty quantity) noexcept
{
    if (id_to_count_.count(id) == 0) {
        printf("No such offer [%llu] to remove.\n", id);
        return false;
    }
    if (quantity > id_to_count_[id]) {
        printf("Unable to remove [%llu] while only [%llu] available.\n", quantity, id_to_count_[id]);
        return false;
    }

    if (unordered_offer_id_.find(id) != unordered_offer_id_.end()) {

        if (asks_.by_id_.find(id) != asks_.by_id_.end()) {
            if (quantity == id_to_count_[id]) {
                close_order_helper(id, asks_, quantity);
            } else {
                id_to_count_[id] -= quantity;
            }
        } else {
            if (quantity == id_to_count_[id]) {
                close_order_helper(id, bids_, quantity);
            } else {
                id_to_count_[id] -= quantity;
            }
        }
    }
    return true;
}

Qty OrderBook::get_l2_size() const noexcept
{
    Qty ret{};
    for (auto qt : id_to_count_) {
        // maybe worth to return bool and size as parameter?
        __builtin_uaddll_overflow(ret, qt.second, &ret);
    }
    return ret;
}

bool OrderBook::store(const std::string& name) const noexcept
{
    std::ofstream file(name);
    if (!file.is_open()) {
        return false;
    }
    nlohmann::json json;

    Price prev{};
    for (auto price = bids_.by_price_.rbegin(); price != bids_.by_price_.rend(); price++) {
        if (prev == price->first) {
            continue;
        }
        auto price_iter = price_to_id_.at(price->first);
        for (auto id : price_iter) {
            nlohmann::json offers_by_price;
            offers_by_price["bid"]["id"] = id;
            offers_by_price["bid"]["price"] = id_to_price_.at(id);
            offers_by_price["bid"]["quantity"] = id_to_count_.at(id);
            json += offers_by_price;
        }
        prev = price->first;
    }
    prev = 0;
    for (auto price = asks_.by_price_.rbegin(); price != asks_.by_price_.rend(); price++) {
        if (prev == price->first) {
            continue;
        }
        auto price_iter = price_to_id_.at(price->first);
        for (auto id : price_iter) {
            nlohmann::json offers_by_price;
            offers_by_price["ask"]["id"] = id;
            offers_by_price["ask"]["price"] = id_to_price_.at(id);
            offers_by_price["ask"]["quantity"] = id_to_count_.at(id);
            json += offers_by_price;
        }
        prev = price->first;
    }

    file << std::setw(4) << json << std::endl;
    return true;
}

bool OrderBook::load(const std::string& name) noexcept
{
    std::ifstream file(name);
    if (!file.is_open()) {
        printf("Unable to open file %s\n", name.c_str());
        return false;
    }
    nlohmann::json json;
    try {
        file >> json;
    } catch (const nlohmann::detail::parse_error& e) {
        printf("Invalid file\n");
        return false;
    }
    create_offer_structure(json);
    return true;
}

void OrderBook::create_offer_structure(const nlohmann::basic_json<>& json) noexcept
{
    for (const auto& str : json) {
        for (const auto& m : str) {
            if (str.begin().key() == "bids") {
                add_offer_to(bids_, m["price"], m["quantity"], m["id"]);
            } else {
                add_offer_to(asks_, m["price"], m["quantity"], m["id"]);
            }
        }
    }
}

bool OrderBook::get_offers_by_price(Price price, std::set<OfferID>*& offers) noexcept
{
    if (price_to_id_.count(price) != 0u) {
        offers = &price_to_id_[price];
        return true;
    }
    return false;
}

std::pair<Price, Qty> OrderBook::get_offer_by_id(OfferID id) const noexcept
{
    if (id_to_price_.count(id) != 0u) {
        return {id_to_price_.at(id), id_to_count_.at(id)};
    }
    return {};
}

std::map<OfferID, std::pair<Price, Qty>> OrderBook::pack_all_data() const noexcept
{
    std::map<OfferID, std::pair<Price, Qty>> ret;
    for (auto id : bids_.by_id_) {
        ret.emplace(id, std::make_pair(id_to_price_.at(id), id_to_count_.at(id)));
    }
    for (auto id : asks_.by_id_) {
        ret.emplace(id, std::make_pair(id_to_price_.at(id), id_to_count_.at(id)));
    }
    return ret;
}

}