#include <gtest/gtest.h>

#include "OrderBook.h"

TEST(add_bid, add_bid) {
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
    l2.print_offers();
//    std::map<OfferId, std::pair<Price, Qty>> subject = l2.pack_all_data();

//    ASSERT_EQ(l2.get_l2_size(), std::accumulate(reference_data_count.begin(), reference_data_count.end(), 0));
//    for (int i = 0; i < reference_data_ids.size(); i++) {
//        ASSERT_EQ(i, reference_data_ids[i]);
//        ASSERT_EQ(subject[i].first, reference_data_price[i]);
//        ASSERT_EQ(subject[i].second, reference_data_count[i]);
//    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
