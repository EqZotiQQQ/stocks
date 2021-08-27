#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/tag.hpp>

using namespace ::boost;
using namespace ::boost::multi_index;

struct Offer {
    OfferID offer_id;
    Price price;
    Count quantity;

    struct OrderedById {};
    struct OrderedByPrice {};
};

typedef multi_index_container
        <Offer,
            indexed_by<
                ordered_unique    <tag<Offer::OrderedById>,    member<Offer, OfferID , &Offer::offer_id>>,
                ordered_non_unique<tag<Offer::OrderedByPrice>, member<Offer, Price,    &Offer::price     >>
            >
        > Store;


class Level2MultiIndexBase {
private:
    Store offers;
public:
    OfferID add_order(Count quantity, Price price, OFFER offer_type) noexcept;
    bool close_order(Count quantity, OfferID id) noexcept;

    auto get_offers_by_price(Price price) const noexcept -> std::vector<OfferById>;
    bool get_offers_by_price(Price price, std::vector<OfferById>*& v) noexcept;

    auto get_offers_by_id(OfferID id) const noexcept -> OfferByPrice;
    bool get_offers_by_id(OfferID id, OfferByPrice*& v) noexcept;

    void print_level2_by_price() const noexcept;
    void print_level2_by_idx() const noexcept;
    Count get_l2_size() const noexcept;


    bool store() const noexcept;
    bool load() noexcept;
};