#praagma once

#include <unordered_map>
#include <map>
#include <forward_list>
#include <vector>

using Price = uint64_t;     // price in cents but i guess i need to switch to something bigger to remove overflow issue
using OfferID = uint64_t;   // id of offer. It
using Count = uint64_t;

struct OfferById {
    OfferID offerId;
    Count quantity;

    bool operator==(const OfferById &rhs) {
        return offerId == rhs.offerId && quantity == rhs.quantity;
    }
};

struct OfferByPrice {
    Price price;
    Count quantity;

    bool operator==(const OfferByPrice &rhs) {
        return price == rhs.price && quantity == rhs.quantity;
    }
};

class Mix {
private:
    std::unordered_map<OfferID, Price> bids_ordered_by_offer_;
    std::unordered_map<OfferID, Price> asks_ordered_by_offer_;

    std::map<Price, std::forward_list<OfferID>> bids_ordered_by_price_;
    std::map<Price, std::forward_list<OfferID>> asks_ordered_by_price_;

    std::unordered_map<OfferID, Count> id_count_map_bids_;
    std::unordered_map<OfferID, Count> id_count_map_asks_;

//    std::map<Price, /*pointer to bids_ordered_by_offer_*/> ???
//    std::map<Price, /*pointer to asks_ordered_by_offer_*/> ???
};

