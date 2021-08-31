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

//    ASSERT_EQ(l2.get_l2_size(), std::accumulate(reference_data_count.begin(), reference_data_count.end(), 0));
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

//    ASSERT_EQ(l2.get_l2_size(), std::accumulate(reference_data_count.begin(), reference_data_count.end(), 0));

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

//    ASSERT_EQ(l2.get_l2_size(), 316);

    for (int i = 0; i < reference_data.size(); i++) {
        ASSERT_EQ(subject[i].price, reference_data[i].price);
        ASSERT_EQ(subject[i].qty, reference_data[i].qty);
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
