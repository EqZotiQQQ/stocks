#pragma once

#include <map>
#include <vector>
#include <tuple>
#include <functional>
#include <cstdint>

#include <nlohmann/json.hpp>

namespace binary_tree_base {

enum class OFFER {
    BID, ASK
};

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


class Level2BinaryTreeBase {
private:
    std::map<Price, std::vector<OfferById>> bids_ordered_by_price_{};
    std::map<Price, std::vector<OfferById>> asks_ordered_by_price_{};

    std::map<OfferID, OfferByPrice> bids_ordered_by_offer_{};
    std::map<OfferID, OfferByPrice> asks_ordered_by_offer_{};

    uint64_t offer_id_{};
public:

    OfferID add_order(Count quantity, Price price, OFFER offer_type) noexcept;

    bool close_order(Count quantity, OfferID id) noexcept;

    auto get_offers_by_price(Price price) const noexcept -> std::vector<OfferById>;

    bool get_offers_by_price(Price price, std::vector<OfferById> *&v) noexcept;

    auto get_offers_by_id(OfferID id) const noexcept -> OfferByPrice;

    bool get_offers_by_id(OfferID id, OfferByPrice *&v) noexcept;

    void print_level2_by_price() const noexcept;

    void print_level2_by_idx() const noexcept;

    Count get_l2_size() const noexcept;


    bool store() const noexcept;

    bool load() noexcept;

private:

    void set_offers_by_type(OFFER offer_type,
                            std::map<Price, std::vector<OfferById>> *&offer_by_price,
                            std::map<OfferID, OfferByPrice> *&offer_by_id);

    bool compare_prices(OFFER offer_type, Price rhs, Price lhs);

    /**
     * В этом методе ask или bid добавляется в стаккан. Если оффера по такой цене
     * ещё небыло, то создаётся новый ключ, если такой оффер уже был - то оффер
     * добавляется в вектор
     * @param offer - это bid или ask
     * @param price - цена сделки
     * @param quantity - количество акций в оффере.
     */
    void add_offer_to(std::map<Price, std::vector<OfferById>> &offer,
                      std::map<OfferID, OfferByPrice> &offer_by_id,
                      Price price,
                      Count quantity,
                      OfferID offer_id) noexcept;

    /**
     * В этом методе обрабатывается ситуация, когда выставляется ask или bid,
     * когда ask выше по цене, чем bid или наоборот, bid меньше по цене чем ask.
     * @param offer - это bid или ask
     * @param quantity - количество акций в оффере.
     */
    Count exchange_existing_offers(OFFER offer_type,
                                   Price price,
                                   Count quantity) noexcept;

    /**
     *
     * @param bid_ask_orders - это bid или ask, отсортированные по цене
     * @param offer_by_id - это bid или ask, отсортированные по id
     * @param quantity - количество акций, которые надо отменить
     * @param id - id оффера
     * @return возвращает успех операции
     */
    bool close_order_support(OFFER offer_type,
                             Count quantity,
                             OfferID id) noexcept;

    void create_offer_structure(const nlohmann::basic_json<> &json) noexcept;

};

}