#include <gtest/gtest.h>

#include "OrderBook.h"

TEST(add_order, add_bid) {
    OrderBookAbseil l2;
    std::vector<OfferId> reference_data_ids;
    std::vector<Price> reference_data_price;
    std::vector<Qty> reference_data_count;

    for (int i = 0; i < 500; i++) {
        Price price = rand() % 100 + 20;
        Price quantity = rand() % 100 + 40;
        reference_data_ids.push_back(l2.add_order(Offer::ASK, price, quantity));
        reference_data_price.push_back(price);
        reference_data_count.push_back(quantity);
    }

    absl::flat_hash_map<OfferId, PriceQty> subject = l2.pack_all_data();

    ASSERT_EQ(l2.get_l2_size(), std::accumulate(reference_data_count.begin(), reference_data_count.end(), 0));
    for (int i = 0; i < reference_data_ids.size(); i++) {
        ASSERT_EQ(i, reference_data_ids[i]);
        ASSERT_EQ(subject[i].price, reference_data_price[i]);
        ASSERT_EQ(subject[i].qty, reference_data_count[i]);
    }
}

TEST(add_order, add_bids)
{
    OrderBookAbseil l2;
    std::vector<OfferId> reference_data_ids;
    std::vector<Price> reference_data_price;
    std::vector<Qty> reference_data_count;

    for (int i = 0; i < 2000; i++) {
        Price price = rand() % 3000 + 50;
        Price quantity = rand() % 1000 + 40;
        reference_data_ids.push_back(l2.add_order(Offer::ASK, price, quantity));
        reference_data_price.push_back(price);
        reference_data_count.push_back(quantity);
    }
    absl::flat_hash_map<OfferId, PriceQty> subject = l2.pack_all_data();

    ASSERT_EQ(l2.get_l2_size(), std::accumulate(reference_data_count.begin(), reference_data_count.end(), 0));

    for (int i = 0; i < reference_data_ids.size(); i++) {
        ASSERT_EQ(i, reference_data_ids[i]);
        ASSERT_EQ(subject[i].price, reference_data_price[i]);
        ASSERT_EQ(subject[i].qty, reference_data_count[i]);
    }
}

TEST(add_order, add_offers_different_types)
{
    OrderBookAbseil l2;
    l2.add_order(Offer::BID, 50, 10);
    l2.add_order(Offer::BID, 25, 10);
    l2.add_order(Offer::BID, 72, 10);
    l2.add_order(Offer::ASK, 50, 115);
    l2.add_order(Offer::ASK, 35, 11);
    l2.add_order(Offer::ASK, 50, 200);

    absl::flat_hash_map<OfferId, PriceQty> subject = l2.pack_all_data();
    absl::flat_hash_map<OfferId, PriceQty> reference_data {
        {2, {72, 10}},
        {3, {50, 95}},
        {4, {35, 11}},
        {5, {50, 200}},
        };

    ASSERT_EQ(l2.get_l2_size(), 316);

    for (int i = 0; i < reference_data.size(); i++) {
        ASSERT_EQ(subject[i].price, reference_data[i].price);
        ASSERT_EQ(subject[i].qty, reference_data[i].qty);
    }
}

TEST(close_order, close_bid)
{
    OrderBookAbseil l2;
    std::vector<OfferId> reference_data_ids;
    std::vector<Price> reference_data_price;
    std::vector<Qty> reference_data_count;

    for (int i = 0; i < 2000; i++) {
        Price price = rand() % 3000 + 50;
        Price quantity = rand() % 1000 + 40;
        reference_data_ids.push_back(l2.add_order(Offer::BID, price, quantity));
        reference_data_price.push_back(price);
        reference_data_count.push_back(quantity);
    }

    ASSERT_EQ(l2.get_l2_size(), std::accumulate(reference_data_count.begin(), reference_data_count.end(), 0));

    ASSERT_TRUE(l2.close_order(reference_data_ids[1], reference_data_count[1]));
    ASSERT_TRUE(l2.close_order(reference_data_ids[5], reference_data_count[5]));
    ASSERT_TRUE(l2.close_order(reference_data_ids[265], reference_data_count[265]));
    ASSERT_TRUE(l2.close_order(reference_data_ids[1000], reference_data_count[1000]));
    ASSERT_TRUE(l2.close_order(reference_data_ids[1999], reference_data_count[1999]));
}


TEST(close_order, close_bid_2)
{
    OrderBookAbseil l2;
    std::vector<OfferId> reference_data_ids;
    std::vector<Price> reference_data_price;
    std::vector<Qty> reference_data_count;

    for (int i = 0; i < 200; i++) {
        Price price = 1; //rand() % 1 + 50;
        Price quantity = rand() % 30 + 40;
        reference_data_ids.push_back(l2.add_order(Offer::BID, price, quantity));
        reference_data_price.push_back(price);
        reference_data_count.push_back(quantity);
    }
    absl::flat_hash_map<OfferId, PriceQty>  subject = l2.pack_all_data();

    ASSERT_EQ(l2.get_l2_size(), std::accumulate(reference_data_count.begin(), reference_data_count.end(), 0));

    for (int i = 0; i < reference_data_ids.size(); i++) {
        l2.close_order(reference_data_ids[i], reference_data_count[i]);
    }
}


TEST(close_order, close_bid_fail_1)
{
    OrderBookAbseil l2;
    Qty quantity = 10;

    OfferId id = l2.add_order(Offer::BID, 50, quantity);

    ASSERT_FALSE(l2.close_order(id, 20));
}

TEST(close_order, close_bid_fail_2)
{
    OrderBookAbseil l2;

    ASSERT_FALSE(l2.close_order(10, 20));
}


TEST(close_order, close_ask_fail_1)
{
    OrderBookAbseil l2;

    Qty quantity = 10;
    l2.add_order(Offer::BID, 50, quantity);

    ASSERT_FALSE(l2.close_order(10, 20));
}

TEST(close_order, close_ask_fail_2)
{
    OrderBookAbseil l2;

    ASSERT_FALSE(l2.close_order(10, 20));
}

TEST(get_offers_by_price, get_prices)
{
    OrderBookAbseil l2;
    std::vector<OfferId> reference_data_ids;
    std::vector<Price> reference_data_price;

    for (int i = 0; i < 3000; i++) {
        Price price = rand() % 300 + 50;
        Price quantity = rand() % 100 + 40;
        reference_data_ids.push_back(l2.add_order(Offer::ASK, price, quantity));
        reference_data_price.push_back(price);
    }
    Price price_ref = reference_data_price.back();
    std::vector<OfferId> test_data;
    for (int i = 0; i < reference_data_ids.size(); i++) {
        if (price_ref == reference_data_price[i]) {
            test_data.push_back(i);
        }
    }

    absl::optional<std::vector<OfferId>> ids = l2.get_offers_by_price(price_ref);

    ASSERT_TRUE(ids.has_value());

    for (int i = 0; i < ids->size(); i++) {
        ASSERT_EQ(ids->at(i), test_data[i]);
    }
}

TEST(get_offers_by_price, get_prices_failure)
{
    OrderBookAbseil l2;
    absl::optional<std::vector<OfferId>> subject = l2.get_offers_by_price(0);

    ASSERT_FALSE(subject.has_value());
    ASSERT_EQ(l2.get_l2_size(), 0);
}

TEST(get_offer_by_id, get_ids1)
{
    OrderBookAbseil l2;
    OfferId id = l2.add_order(Offer::BID, 50, 10);
    PriceQty sample{
        .price = 50,
        .qty = 10,
        .type = Offer::BID
    };
    absl::optional<PriceQty> subject = l2.get_order_by_id(id);
    ASSERT_TRUE(subject.has_value());
    ASSERT_EQ(l2.get_l2_size(), 10);
    ASSERT_EQ(subject.value(), sample);
}

TEST(get_offer_by_id, get_ids2)
{
    OrderBookAbseil l2;

    absl::optional<PriceQty> subject = l2.get_order_by_id(0);

    ASSERT_FALSE(subject.has_value());
    ASSERT_EQ(l2.get_l2_size(), 0);
}

TEST(get_l2_size, get_number_of_l2)
{
    OrderBookAbseil l2;
    l2.add_order(Offer::BID, 50, 10);
    l2.add_order(Offer::BID, 20, 15);

    ASSERT_EQ(l2.get_l2_size(), 25);
}

TEST(get_l2_size, get_number_of_empty_l2)
{
    OrderBookAbseil l2;

    ASSERT_EQ(l2.get_l2_size(), 0);
}

TEST(store, store_json_test)
{
    OrderBookAbseil l2;

    std::vector<OfferId> reference_data_ids;
    std::vector<Price> reference_data_price;
    std::vector<Qty> reference_data_count;

    for (int i = 0; i < 5000; i++) {
        Price price = rand() % 3000 + 50;
        Price quantity = rand() % 1000 + 40;
        reference_data_ids.push_back(l2.add_order(Offer::BID, price, quantity));
        reference_data_price.push_back(price);
        reference_data_count.push_back(quantity);
    }

    ASSERT_TRUE(l2.store());
}

TEST(load, load_json_test)
{
    OrderBookAbseil l2;

    ASSERT_TRUE(l2.load());
    ASSERT_TRUE(l2.store("load_json_test.json"));
}

TEST(load, load_json_test_failure)
{
    OrderBookAbseil l2;

    ASSERT_FALSE(l2.load("some_name.json"));
}

TEST(store, store_load_json_test)
{
    OrderBookAbseil l2;

    for (int i = 0; i < 5000; i++) {
        Price price = rand() % 3000 + 50;
        Price quantity = rand() % 1000 + 40;
        l2.add_order(Offer::BID, price, quantity);
    }
    ASSERT_TRUE(l2.store());

    OrderBookAbseil l2_test;
    ASSERT_TRUE(l2_test.load());

    absl::flat_hash_map<OfferId, PriceQty> l2_data = l2.pack_all_data();
    absl::flat_hash_map<OfferId, PriceQty> l2_test_data = l2_test.pack_all_data();

    ASSERT_EQ(l2.get_l2_size(), l2_test.get_l2_size());

    for (int i = 0; i < l2_data.size(); i++) {
        ASSERT_EQ(l2_data[i].price, l2_test_data[i].price);
        ASSERT_EQ(l2_data[i].qty, l2_test_data[i].qty);
        ASSERT_EQ(l2_data[i].type, l2_test_data[i].type);
    }

}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
