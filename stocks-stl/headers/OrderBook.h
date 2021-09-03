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
    OrderBook();
    /***
     * Place order into order book
     * @param order_type ask or bid order
     * @param price price for order
     * @param quantity number of orders
     * @return id of order
     */
    OrderID add_order(ORDER_TYPE order_type, Price price, Qty quantity) noexcept;

    /***
     * close order on stock
     * @param id id of order
     * @param quantity number of orders that should be closed
     * @return success of operation
     */
    bool close_order(OrderID id, Qty quantity) noexcept;

    /***
     * make snapshot to json
     * @param name name of json file
     * @return success of operation
     */
    bool store(const std::string& name = "stocks-stl.json") noexcept;

    /***
     * load snapshot from file
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
     * @return pair price-count of order
     */
    std::pair<Price, Qty> get_order_by_id(OrderID id) const noexcept;

    /***
     * method for debug and tests
     * @return returns snapshot of data
     */
    std::map<OrderID, std::pair<Price, Qty>> pack_all_data() const noexcept;

    /***
     * get number of orders in level2
     * @return returns number of offers in level2
     */

    Qty get_l2_size() const noexcept;

private:

    Orders asks_, bids_;

    // O(1) check speed. Return false if insert failed
    std::unordered_set<OrderID> unordered_order_id_;

    // provides O(1) access to count
    std::unordered_map<OrderID, PriceQty> id_to_data_;

    // provides O(1) access to id
    std::unordered_map<Price, std::set<OrderID>> price_to_id_;

    // order_id. Might be overflowed and probably there should be used something that can't be overloaded.
    OrderID order_id_{};

    Qty size_ {};

    /***
     * simply add order to level2.
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
     * unpack data from and push them to level2
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