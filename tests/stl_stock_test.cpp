#include "tools.h"

using namespace stl;

TEST(add_order_stl, add_asks)
{
    OrderBook l2;
    std::vector<OrderID> reference_data_ids;
    std::vector<Price> reference_data_price;
    std::vector<Qty> reference_data_count;

    for (int i = 0; i < 2000; i++) {
        Price price = rand() % 3000 + 50;
        Price quantity = rand() % 1000 + 40;
        reference_data_ids.push_back(l2.add_order(ORDER_TYPE::ASK, price, quantity));
        reference_data_price.push_back(price);
        reference_data_count.push_back(quantity);
    }
    std::map<OrderID, std::pair<Price, Qty>> subject = l2.pack_all_data();

    ASSERT_EQ(l2.get_l2_size(), std::accumulate(reference_data_count.begin(), reference_data_count.end(), 0));
    for (int i = 0; i < reference_data_ids.size(); i++) {
        ASSERT_EQ(i, reference_data_ids[i]);
        ASSERT_EQ(subject[i].first, reference_data_price[i]);
        ASSERT_EQ(subject[i].second, reference_data_count[i]);
    }
}

TEST(add_order_stl, add_bids)
{
    OrderBook l2;
    std::vector<OrderID> reference_data_ids;
    std::vector<Price> reference_data_price;
    std::vector<Qty> reference_data_count;

    for (int i = 0; i < 2000; i++) {
        Price price = rand() % 3000 + 50;
        Price quantity = rand() % 1000 + 40;
        reference_data_ids.push_back(l2.add_order(ORDER_TYPE::BID, price, quantity));
        reference_data_price.push_back(price);
        reference_data_count.push_back(quantity);
    }
    std::map<OrderID, std::pair<Price, Qty>> subject = l2.pack_all_data();

    ASSERT_EQ(l2.get_l2_size(), std::accumulate(reference_data_count.begin(), reference_data_count.end(), 0));

    for (int i = 0; i < reference_data_ids.size(); i++) {
        ASSERT_EQ(i, reference_data_ids[i]);
        ASSERT_EQ(subject[i].first, reference_data_price[i]);
        ASSERT_EQ(subject[i].second, reference_data_count[i]);
    }
}

TEST(add_order_stl, add_orders_different_types)
{
    OrderBook l2;
    l2.add_order(ORDER_TYPE::BID, 50, 10);
    l2.add_order(ORDER_TYPE::BID, 25, 10);
    l2.add_order(ORDER_TYPE::BID, 72, 10);
    l2.add_order(ORDER_TYPE::ASK, 50, 115);
    l2.add_order(ORDER_TYPE::ASK, 35, 11);
    l2.add_order(ORDER_TYPE::ASK, 50, 200);

    std::map<OrderID, std::pair<Price, Qty>> subject = l2.pack_all_data();
    std::map<OrderID, std::pair<Price, Qty>> reference_data{
        {2, {72, 10}},
        {3, {50, 95}},
        {4, {35, 11}},
        {5, {50, 200}},
    };

    ASSERT_EQ(l2.get_l2_size(), 316);

    for (int i = 0; i < reference_data.size(); i++) {
        ASSERT_EQ(subject[i].first, reference_data[i].first);
        ASSERT_EQ(subject[i].second, reference_data[i].second);
    }
}

TEST(close_order_stl, close_ask)
{
    OrderBook l2;
    std::vector<OrderID> reference_data_ids;
    std::vector<Price> reference_data_price;
    std::vector<Qty> reference_data_count;

    for (int i = 0; i < 2000; i++) {
        Price price = rand() % 3000 + 50;
        Price quantity = rand() % 1000 + 40;
        reference_data_ids.push_back(l2.add_order(ORDER_TYPE::ASK, price, quantity));
        reference_data_price.push_back(price);
        reference_data_count.push_back(quantity);
    }

    ASSERT_EQ(l2.get_l2_size(), std::accumulate(reference_data_count.begin(), reference_data_count.end(), 0));

    for (int i = 0; i < reference_data_ids.size(); i++) {
        ASSERT_TRUE(l2.close_order(reference_data_ids[i], reference_data_count[i]));
    }
}

TEST(close_order_stl, close_bid)
{
    OrderBook l2;
    std::vector<OrderID> reference_data_ids;
    std::vector<Price> reference_data_price;
    std::vector<Qty> reference_data_count;

    for (int i = 0; i < 2000; i++) {
        Price price = rand() % 3000 + 50;
        Price quantity = rand() % 1000 + 40;
        reference_data_ids.push_back(l2.add_order(ORDER_TYPE::BID, price, quantity));
        reference_data_price.push_back(price);
        reference_data_count.push_back(quantity);
    }

    ASSERT_EQ(l2.get_l2_size(), std::accumulate(reference_data_count.begin(), reference_data_count.end(), 0));

    for (int i = 0; i < reference_data_ids.size(); i++) {
        ASSERT_TRUE(l2.close_order(reference_data_ids[i], reference_data_count[i]));
    }
}

TEST(close_order_stl, close_bid_2)
{
    OrderBook l2;
    std::vector<OrderID> reference_data_ids;
    std::vector<Price> reference_data_price;
    std::vector<Qty> reference_data_count;

    for (int i = 0; i < 200; i++) {
        Price price = rand() % 1 + 50;
        Price quantity = rand() % 30 + 40;
        reference_data_ids.push_back(l2.add_order(ORDER_TYPE::BID, price, quantity));
        reference_data_price.push_back(price);
        reference_data_count.push_back(quantity);
    }
    std::map<OrderID, std::pair<Price, Qty>> subject = l2.pack_all_data();

    ASSERT_EQ(l2.get_l2_size(), std::accumulate(reference_data_count.begin(), reference_data_count.end(), 0));

    for (int i = 0; i < reference_data_ids.size(); i++) {
        l2.close_order(reference_data_ids[i], reference_data_count[i]);
    }
}

TEST(close_order_stl, close_bid_fail_1)
{
    OrderBook l2;
    Qty quantity = 10;

    OrderID id = l2.add_order(ORDER_TYPE::BID, 50, quantity);

    ASSERT_FALSE(l2.close_order(id, 20));
}

TEST(close_order_stl, close_bid_fail_2)
{
    OrderBook l2;

    ASSERT_FALSE(l2.close_order(10, 20));
}

TEST(close_order_stl, close_ask_fail_1)
{
    OrderBook l2;

    Qty quantity = 10;
    l2.add_order(ORDER_TYPE::BID, 50, quantity);

    ASSERT_FALSE(l2.close_order(10, 20));
}

TEST(close_order_stl, close_ask_fail_2)
{
    OrderBook l2;

    ASSERT_FALSE(l2.close_order(10, 20));
}

TEST(get_orders_by_price_stl, get_prices)
{
    OrderBook l2;
    std::vector<OrderID> reference_data_ids;
    std::vector<Price> reference_data_price;
    std::set<OrderID>* ids = nullptr;
    for (int i = 0; i < 3000; i++) {
        Price price = rand() % 300 + 50;
        Price quantity = rand() % 100 + 40;
        reference_data_ids.push_back(l2.add_order(ORDER_TYPE::ASK, price, quantity));
        reference_data_price.push_back(price);
    }
    Price price_ref = reference_data_price.back();
    std::vector<OrderID> test_data;
    for (int i = 0; i < reference_data_ids.size(); i++) {
        if (price_ref == reference_data_price[i]) {
            test_data.push_back(i);
        }
    }

    l2.get_orders_by_price(price_ref, ids);

    auto sampliter = test_data.begin();
    for (auto subjiter = ids->begin(); subjiter != ids->end(); subjiter++, sampliter++) {
        ASSERT_EQ(*sampliter, *subjiter);
    }
}

TEST(get_orders_by_price_stl, get_prices_failure)
{
    OrderBook l2;
    std::set<OrderID>* subject = nullptr;

    ASSERT_FALSE(l2.get_orders_by_price(0, subject));
    ASSERT_EQ(l2.get_l2_size(), 0);
}

TEST(get_order_by_id_stl, get_ids1)
{
    OrderBook l2;
    l2.add_order(ORDER_TYPE::BID, 50, 10);
    stl::PriceQty sample{.price = 50, .qty =10};
    stl::PriceQty* subject = nullptr;
    l2.get_order_by_id(0, subject);

    ASSERT_EQ(l2.get_l2_size(), 10);
    ASSERT_EQ(*subject, sample);
}

TEST(get_order_by_id_stl, get_ids2)
{
    OrderBook l2;

    stl::PriceQty* subject = nullptr;

    ASSERT_FALSE(l2.get_order_by_id(0, subject));
}

TEST(get_l2_size_stl, get_number_of_l2)
{
    OrderBook l2;
    l2.add_order(ORDER_TYPE::BID, 50, 10);
    l2.add_order(ORDER_TYPE::BID, 20, 15);

    ASSERT_EQ(l2.get_l2_size(), 25);
}

TEST(get_l2_size_stl, get_number_of_empty_l2)
{
    OrderBook l2;

    ASSERT_EQ(l2.get_l2_size(), 0);
}

TEST(store_stl, store_json_test)
{
    OrderBook l2;

    std::vector<OrderID> reference_data_ids;
    std::vector<Price> reference_data_price;
    std::vector<Qty> reference_data_count;

    for (int i = 0; i < 2000; i++) {
        Price price = rand() % 3000 + 50;
        Price quantity = rand() % 1000 + 40;
        reference_data_ids.push_back(l2.add_order(ORDER_TYPE::BID, price, quantity));
        reference_data_price.push_back(price);
        reference_data_count.push_back(quantity);
    }

    ASSERT_TRUE(l2.store());
}

TEST(load_stl, load_json_test)
{
    OrderBook l2;

    ASSERT_TRUE(l2.load());
    ASSERT_TRUE(l2.store("load_json_test.json"));
}

TEST(load_stl, load_json_test_failure)
{
    OrderBook l2;

    ASSERT_FALSE(l2.load("some_name.json"));
}

TEST(store_stl, store_load_json_test)
{
    OrderBook l2;

    for (int i = 0; i < 5000; i++) {
        Price price = rand() % 3000 + 50;
        Price quantity = rand() % 1000 + 40;
        l2.add_order(ORDER_TYPE::BID, price, quantity);
    }
    ASSERT_TRUE(l2.store("stl_data.json"));

    OrderBook l2_test;
    ASSERT_TRUE(l2_test.load("stl_data.json"));
    l2_test.store("stl_data2.json");

    std::map<OrderID, std::pair<Price, Qty>> l2_data = l2.pack_all_data();
    std::map<OrderID, std::pair<Price, Qty>> l2_test_data = l2_test.pack_all_data();

    ASSERT_EQ(l2.get_l2_size(), l2_test.get_l2_size());

    auto iter_l2_data = l2_data.begin();
    auto iter_l2_test = l2_test_data.begin();
    while(iter_l2_data != l2_data.end()) {
        ASSERT_EQ(iter_l2_data->first, iter_l2_test->first);
        ASSERT_EQ(iter_l2_data->second.first, iter_l2_test->second.first);
        ASSERT_EQ(iter_l2_data->second.second, iter_l2_test->second.second);
        iter_l2_data++; iter_l2_test++;
    }
}

TEST(trade_stl, trade)
{
    OrderBook l2;
    for (int i = 0; i < 50; i++) {
        Price price = rand() % 3000 + 50;
        Price quantity = rand() % 1000 + 40;
        if (i % 2 == 0) {
            l2.add_order(ORDER_TYPE::ASK, price, quantity);
        } else {
            l2.add_order(ORDER_TYPE::BID, price, quantity);
        }
    }
    l2.store();
}

int run_stl_tests(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
