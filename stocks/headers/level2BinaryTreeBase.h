#pragma once

#include <map>
#include <vector>
#include <tuple>
#include <functional>
#include <cstdint>
#include "../../cmake-build-debug/_deps/json-src/include/nlohmann/json.hpp"

using namespace std;

class Level2BinaryTreeBase {
    using Price = uint64_t;     // price in cents but i guess i need to switch to something bigger to remove overflow issue
    using OfferID = uint64_t;   // id of offer. It
    using Count = uint64_t;
    using Callback = std::function<void()>; // Этот колбек, можно вызвывать по отработке оффера
private:
    std::map<Price, vector<pair<OfferID, Count>>> bids_{};
    std::map<Price, vector<pair<OfferID, Count>>> asks_{};

    std::map<OfferID, pair<Price, Count>> bids_by_offer_{};
    std::map<OfferID, pair<Price, Count>> asks_by_offer_{};

    uint64_t offer_id {};
public:
    Level2BinaryTreeBase();

    OfferID add_order(int quantity, Price price, bool isBid);
    bool close_order(unsigned int quantity, OfferID id);

    auto get_offers_by_price(Price price) -> std::vector<std::pair<OfferID, Count>>;
    auto get_offers_by_id(OfferID id) -> std::pair<Price, Count>;

    void print_level2_by_price();
    void print_level2_by_idx();
    Count get_l2_size();

    bool store();
    bool load();

private:
    /**
     * В этом методе ask или bid добавляется в стаккан. Если оффера по такой цене
     * ещё небыло, то создаётся новый ключ, если такой оффер уже был - то оффер
     * добавляется в вектор
     * @param offer - это bid или ask
     * @param price - цена сделки
     * @param quantity - количество акций в оффере.
     */
    void add_offer_to(std::map<Price, vector<pair<OfferID, Count>>>& offer,
                      std::map<OfferID, pair<Price, Count>>& offer_by_id,
                      Price price,
                      int quantity,
                      int offer_id);

    /**
     * В этом методе обрабатывается ситуация, когда выставляется ask или bid,
     * когда ask выше по цене, чем bid или наоборот, bid меньше по цене чем ask.
     * @param offer - это bid или ask
     * @param quantity - количество акций в оффере.
     */
    void exchange_existing_offers(std::map<Price, vector<pair<OfferID, Count>>>& offer,
                                  std::map<OfferID, pair<Price, Count>>& offer_by_id,
                                  int& quantity);

    /**
     *
     * @param bid_ask_orders - это bid или ask, отсортированные по цене
     * @param offer_by_id - это bid или ask, отсортированные по id
     * @param quantity - количество акций, которые надо отменить
     * @param id - id оффера
     * @return возвращает успех операции
     */
    static bool close_order_support(std::map<Price, vector<pair<OfferID, Count>>>& bid_ask_orders,
                                    std::map<OfferID, pair<Price, Count>>& offer_by_id,
                                    int quantity,
                                    int id);

    void create_offer_structure(const nlohmann::basic_json<>& json);
};