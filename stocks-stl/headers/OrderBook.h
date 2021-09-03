#pragma once

#include "Order.h"

#include <nlohmann/json.hpp>

#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace stl {

using Price
    = unsigned long long int; // price in cents but i guess i need to switch to something bigger to remove overflow issue
using OrderID = unsigned long long int; // id of order. It
using Qty = unsigned long long int;

struct PriceQty {
    Price price;
    Qty qty;
    bool operator==(const PriceQty& rhs) const {
        return price == rhs.price && qty == rhs.qty;
    }
};

/***
 * OrderBook that stores active orders.
 * Implemented using stl.
 *
 * Few things that could make it faster:
 * * swisstables over unordered_map
 * * facebook's stl analog
 * * self implemented data structures
 */
class OrderBook
{
public:
    /***
     * initializes names
     */
    OrderBook();
    /***
     * Push order into order book
     * @param order_type ask or bid order
     * @param price price for order
     * @param quantity number of orders
     * @return id of order
     */
    OrderID add_order(ORDER_TYPE order_type, Price price, Qty quantity) noexcept;

    /***
     * close order in order book
     * @param id id of order
     * @param quantity number of orders that should be closed
     * @return success of operation
     */
    bool close_order(OrderID id, Qty quantity) noexcept;

    /***
     * store snapshot to json file
     * @param name name of json file
     * @return success of operation
     */
    bool store(const std::string& name = "stocks-stl.json") noexcept;

    /***
     * load json from file
     * @param name file load from
     * @return success of operation
     */
    bool load(const std::string& name = "stocks-stl.json") noexcept;

    /***
     * get as parameter set of orders id filtered by price
     * @param price price filter
     * @param orders set of order id
     * @return success of operation
     */
    bool get_orders_by_price(Price price, std::set<OrderID>*& orders) noexcept;

    /***
     * get price-count by id
     * @param id if of order
     * @param order stores truly return value to this pointer
     * @return success of operation
     */
    bool get_order_by_id(OrderID id, PriceQty*& order) noexcept;

    /***
     * method for debug and tests
     * @return returns snapshot of data
     */
    std::map<OrderID, std::pair<Price, Qty>> pack_all_data() const noexcept;

    /***
     * get orders quantity in order book
     * @return returns number of orders in level2
     */
    Qty get_l2_size() const noexcept;

private:

    // contains data related with each order type
    Orders asks_, bids_;

    // O(1) check speed. Return false if insert failed
    std::unordered_set<OrderID> unordered_order_id_;

    // provides all data related to order id
    std::unordered_map<OrderID, PriceQty> id_to_data_;

    // provides all orders for same price
    std::unordered_map<Price, std::set<OrderID>> price_to_id_;

    // id of last order that was pushed to order book
    OrderID order_id_{};

    // count of all orders in order book
    Qty size_ {};

    /***
     * add order to order book.
     * @param order_type
     * @param price order price
     * @param quantity number of orders
     * @param id order id
     */
    void add_order_to(Orders& orders, Price price, Qty quantity, OrderID id) noexcept;

    /***
     * closes existing asks if bid is less expensive or closes bid if ask is more expensive.
     * @param order_type
     * @param price order price
     * @param quantity number of orders
     * @return
     */
    Qty exchange_orders(Orders& orders, Price price, Qty quantity) noexcept;

    /***
     * unpack data from json and push them to order book
     * @param json file
     */
    void create_order_structure(const nlohmann::basic_json<>& json) noexcept;

    /***
     * closes the order
     * @param id
     * @param orders
     * @param order_quantity number of orders
     * @return return number of order then left after compensations opposite
     */
    void close_order_helper(Orders& orders, OrderID id, Price price);
};

}
