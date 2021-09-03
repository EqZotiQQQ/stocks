#include "OrderBook.h"

#include <fstream>
#include <iomanip>
#include <iostream>

namespace stl {

OrderID OrderBook::add_order(ORDER_TYPE order_type, Price price, Qty quantity) noexcept
{
    if (order_type == ORDER_TYPE::BID) {
        quantity = exchange_orders(asks_, price, quantity);
        add_order_to(bids_, price, quantity, order_id_);
    } else {
        quantity = exchange_orders(bids_, price, quantity);
        add_order_to(asks_, price, quantity, order_id_);
    }
    return order_id_++;
}

void OrderBook::add_order_to(Orders& orders, Price price, Qty quantity, OrderID id) noexcept
{
    if (quantity != 0u) {
        if (!orders.unordered_price_.insert(price).second) {
            unordered_order_id_.emplace(id);
            price_to_id_[price].insert(id);
            orders.by_price_[price]++;
        } else {
            price_to_id_.emplace(price, std::set<OrderID>{id});
            orders.by_price_.emplace(price, 1);
        }
        unordered_order_id_.emplace(id);
        orders.by_id_.insert(id);

        id_to_data_.emplace(id, PriceQty{.price = price, .qty = quantity});

        size_ += quantity;
    }
}

Qty OrderBook::exchange_orders(Orders& orders, Price order_price, Qty order_quantity) noexcept
{
    if (order_quantity == 0 || orders.by_id_.empty()) {
        return order_quantity;
    }

    Price target_price;
    bool exchange_condition;

    if (orders.type == ORDER_TYPE::BID) {
        target_price =   bids_.by_price_.begin()->first;
        exchange_condition = order_price >= target_price;
    } else {
        target_price =   asks_.by_price_.rbegin()->first;
        exchange_condition = target_price >= order_price;
    }

    if (!exchange_condition) {
        return order_quantity;
    }

    OrderID target_id =  *price_to_id_[target_price].begin();
    Qty target_qty =     id_to_data_[target_id].qty;

    if (target_qty > order_quantity) {
        id_to_data_[target_id].qty -= order_quantity;
        size_ -= order_quantity;
        order_quantity = 0;
        return order_quantity;
    } else {
        close_order_helper(orders, target_id, target_price);
        order_quantity -= target_qty;
        size_ -= target_qty;
    }
    return exchange_orders(orders, order_price, order_quantity);
}

void OrderBook::close_order_helper(Orders& orders, OrderID id, Price price)
{
    orders.by_price_[price]--;
    if (orders.by_price_[price] == 0) {
        orders.unordered_price_.erase(price);
        orders.by_price_.erase(price);
        price_to_id_.erase(price);
    } else {
        auto iter = price_to_id_[price].begin();
        price_to_id_[price].erase(iter);
    }
    unordered_order_id_.erase(id);
    id_to_data_.erase(id);
    orders.by_id_.erase(id);
}

bool OrderBook::close_order(OrderID id, Qty quantity) noexcept
{
    if (id_to_data_.count(id) == 0) {
        printf("No such order [%llu] to remove.\n", id);
        return false;
    }
    if (quantity > id_to_data_[id].qty) {
        printf("Unable to remove [%llu] while only [%llu] available.\n", quantity, id_to_data_[id].qty);
        return false;
    }

    if (unordered_order_id_.find(id) != unordered_order_id_.end()) {
        if (quantity == id_to_data_[id].qty) {

            Price target_price = id_to_data_[id].price;
            size_ -= quantity;
            if (asks_.by_id_.find(id) != asks_.by_id_.end()) {
                close_order_helper(asks_, id, target_price);
            } else {
                close_order_helper(bids_, id, target_price);
            }
        } else {
            id_to_data_[id].qty -= quantity;
        }
    }
    return true;
}

Qty OrderBook::get_l2_size() const noexcept
{
    return size_;
}

bool OrderBook::store(const std::string& name) noexcept
{
    std::ofstream file(name);
    if (!file.is_open()) {
        return false;
    }
    nlohmann::json json;
    auto items = {&bids_, &asks_};
    for (const auto& order: items) {
        for (auto price = order->by_price_.rbegin(); price != order->by_price_.rend(); price++) {
            std::set<OrderID>* ids_by_price = nullptr;
            if (!get_orders_by_price(price->first, ids_by_price)) {
                return false;
            }
            for (OrderID id : *ids_by_price) {
                nlohmann::json orders_by_price;
                orders_by_price[order->order_type]["id"] = id;
                orders_by_price[order->order_type]["price"] = id_to_data_[id].price;
                orders_by_price[order->order_type]["quantity"] = id_to_data_[id].qty;
                json += orders_by_price;
            }
        }
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
    create_order_structure(json);
    return true;
}

void OrderBook::create_order_structure(const nlohmann::basic_json<>& json) noexcept
{
    for (const auto& str : json) {
        for (const auto& m : str) {
            if (str.begin().key() == "bids") {
                add_order_to(bids_, m["price"], m["quantity"], m["id"]);
            } else {
                add_order_to(asks_, m["price"], m["quantity"], m["id"]);
            }
        }
    }
}

bool OrderBook::get_orders_by_price(Price price, std::set<OrderID>*& orders) noexcept
{
    if (price_to_id_.count(price) != 0u) {
        orders = &price_to_id_.at(price);
        return true;
    }
    return false;
}

bool OrderBook::get_order_by_id(OrderID id, PriceQty*& order) noexcept
{
    if (id_to_data_.count(id) != 0u) {
        order = &id_to_data_[id];
        return true;
    }
    return false;
}

std::map<OrderID, std::pair<Price, Qty>> OrderBook::pack_all_data() const noexcept
{
    std::map<OrderID, std::pair<Price, Qty>> ret;

    auto items = {&bids_, &asks_};
    for (const auto& order: items) {
        for (auto id : order->by_id_) {
            ret.emplace(id, std::make_pair(id_to_data_.at(id).price, id_to_data_.at(id).qty));
        }
    }
    return ret;
}

OrderBook::OrderBook()
{
    asks_.order_type = "ask";
    asks_.type = ORDER_TYPE::ASK;
    bids_.order_type = "bid";
    bids_.type = ORDER_TYPE::BID;
}

}
