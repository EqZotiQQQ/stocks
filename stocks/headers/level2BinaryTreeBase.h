#pragma once

#include <map>
#include <vector>
#include <tuple>

using namespace std;

class Level2BinaryTreeBase {
    using Price = uint64_t;     // price in cents but i guess i need to switch to something bigger to remove overflow issue
    using OfferID = uint64_t;   // id of offer. It
    using Count = uint64_t;
    using Callback = std::function<void()>;
private:
    std::map<Price, vector<pair<OfferID, Count>>> bids_{};
    std::map<Price, vector<pair<OfferID, Count>>> asks_{};
    uint64_t offer_id {};
public:
    Level2BinaryTreeBase();

    auto add_order(int quantity, Price price, bool isBid) -> OfferID;
    bool close_order(unsigned int quantity, OfferID id);


    bool get_offers_by_price(Price price, std::vector<std::pair<OfferID, Count>>*& vec);
    bool get_offers_by_id(OfferID id, std::pair<OfferID, Count>*& offer_id);


    void print_level2_by_price();
    void print_level2_by_idx();
    auto get_l2_size() -> OfferID;
};