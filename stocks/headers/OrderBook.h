#pragma once

#include <nlohmann/json.hpp>

#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Price
    = unsigned long long int; // price in cents but i guess i need to switch to something bigger to remove overflow issue
using OfferID = unsigned long long int; // id of offer. It
using Count = unsigned long long int;

/***
 * offer type
 */
enum class OFFER { BID, ASK };

/***
 * OrderBook that stores active offers.
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
     * Place offer into level2
     * @param offer_type ask or bid offer
     * @param price price for offer
     * @param quantity number of offers
     * @return id of offer
     */
    OfferID add_order(OFFER offer_type, Price price, Count quantity) noexcept;

    /***
     * close order on stock
     * @param id id of offer
     * @param quantity number of offers that should be closed
     * @return success of operation
     */
    bool close_order(OfferID id, Count quantity) noexcept;

    /***
     * make snapshot to json
     * @param name name of json file
     * @return success of operation
     */
    bool store(const std::string& name = "stocks.json") const noexcept;

    /***
     * load snapshot from file
     * @param name file load from
     * @return success of operation
     */
    bool load(const std::string& name = "stocks.json") noexcept;

    /***
     * get as parameter set of offers id filtered by price
     * @param price price filter
     * @param offers set of offer id
     * @return success of operation
     */
    bool get_offers_by_price(Price price, std::set<OfferID>*& offers) noexcept;

    /***
     * get price-count by id
     * @param id if of offer
     * @return pair price-count of offer
     */
    std::pair<Price, Count> get_offer_by_id(OfferID id) const noexcept;

    /***
     * method for debug and tests
     * @return returns snapshot of data
     */
    std::map<OfferID, std::pair<Price, Count>> pack_all_data() const noexcept;

    /***
     * get number of offers in level2
     * @return returns number of offers in level2
     */
    Count get_l2_size() const noexcept;

private:
    /***
     * Helper object to keep ref on objects in private methods.
     * Because of asks and bids pretty similar big amount of code
     * can be removed using this helper class.
     */
    struct OfferSupporter_t
    {
        std::set<OfferID>& offers_ordered_by_offer_id_;
        std::map<Price, Count>& offers_ordered_by_price_;
        std::unordered_set<Price>& unordered_offer_price_;
    };

    // contains unique ids in order. Use them like keys
    std::set<OfferID> bids_ordered_by_offer_id_;
    std::set<OfferID> asks_ordered_by_offer_id_;

    // contain prices count of offer for those prices in order. Use them like keys
    std::map<Price, Count> bids_ordered_by_price_;
    std::map<Price, Count> asks_ordered_by_price_;

    // O(1) check speed. Return false if insert failed
    std::unordered_set<Price> unordered_bid_price_;
    std::unordered_set<Price> unordered_ask_price_;

    // O(1) check speed. Return false if insert failed
    std::unordered_map<OfferID, OFFER> unordered_offer_id_;

    // provides O(1) access to count
    std::unordered_map<OfferID, Count> id_to_count_;
    std::unordered_map<OfferID, Price> id_to_price_;

    // provides O(1) access to id
    std::unordered_map<Price, std::set<OfferID>> price_to_id_;

    // offer_id. Might be overflowed and probably there should be used something that can't be overloaded.
    OfferID offer_id_{};

    /***
     * Fills out the structure depending on the type of order.
     * @param offer_type
     * @param swap_offer_type for close offers
     * @return struct of object that contains offers based on
     */
    OfferSupporter_t identify_offer_helper(OFFER offer_type, bool swap_offer_type = false) noexcept;

    /***
     * simply add offer to level2.
     * @param offer_type
     * @param price offer price
     * @param quantity number of offers
     * @param id offer id
     */
    void add_offer_to(OFFER offer_type, Price price, Count quantity, OfferID id) noexcept;

    /***
     * closes existing asks if bid is less expensive or closes bid if ask is more expensive.
     * @param offer_type
     * @param price offer price
     * @param quantity number of offers
     * @return
     */
    Count exchange_offers(OFFER offer_type, Price price, Count quantity) noexcept;

    /***
     * unpack data from and push them to level2
     * @param json file
     */
    void create_offer_structure(const nlohmann::basic_json<>& json) noexcept;

    /***
     * closes the offer
     * @param id
     * @param orders
     * @param offer_quantity number of offers
     * @return return number of offer then left after compensations opposite
     */
    Count close_order_helper(OfferID id, OfferSupporter_t orders, Count offer_quantity);
};
