#include "level2Interface.h"

#include "gtest/gtest.h"

#include <iostream>

using Price = uint64_t;     // price in cents but i guess i need to switch to something bigger to remove overflow issue
using OfferID = uint64_t;   // id of offer. It
using Count = uint64_t;
using Callback = std::function<void()>;

constexpr int print_type{1};

TEST(Test1, create_level2_and_add_some_orders_with_same_prices) {
    Level2Interface l2;
    l2.add_order(4, 50, 1);
    l2.add_order(2, 25, 1);
    l2.add_order(5, 72, 1);
    l2.add_order(1, 72, 1);
    ASSERT_EQ(l2.get_l2_size(), 12);
    std::vector<std::pair<OfferID, Count>> test {{1, 2}};
    ASSERT_EQ(l2.get_offers_by_price(25), test);
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test1, add_bids_then_add_asks1) {
    Level2Interface l2;
    l2.add_order(10, 50, 1);
    l2.add_order(10, 25, 1);
    l2.add_order(10, 72, 1);
    l2.add_order(2, 35, 0);
    l2.add_order(1, 65, 0);
    std::vector<std::pair<OfferID, Count>> test {{2, 10}};
    ASSERT_EQ(l2.get_l2_size(), 27);
    ASSERT_EQ(l2.get_offers_by_price(72), test);
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test1, add_bids_then_add_asks2) {
    Level2Interface l2;
    l2.add_order(10, 50, 1);
    l2.add_order(10, 25, 1);
    l2.add_order(10, 72, 1);
    l2.add_order(11, 35, 0);
    std::vector<std::pair<OfferID, Count>> test {{2, 10}};
    ASSERT_EQ(l2.get_offers_by_price(72), test);
    std::vector<std::pair<OfferID, Count>> test2 {{3, 1}};
    ASSERT_EQ(l2.get_offers_by_price(35), test2);
    ASSERT_EQ(l2.get_l2_size(), 21);
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test1, add_bids_then_add_asks3) {
    Level2Interface l2;
    l2.add_order(10, 50, 1);
    l2.add_order(10, 25, 1);
    l2.add_order(10, 72, 1);
    l2.add_order(11, 65, 0);
    std::vector<std::pair<OfferID, Count>> test {{0, 9}};
    ASSERT_EQ(l2.get_l2_size(), 19);
    ASSERT_EQ(l2.get_offers_by_price(50), test);
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test1, add_bids_then_add_asks4) {
    Level2Interface l2;
    l2.add_order(10, 50, 1);
    l2.add_order(10, 25, 1);
    l2.add_order(10, 72, 1);
    l2.add_order(110, 80, 0);
    ASSERT_EQ(l2.get_l2_size(), 80);
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test1, add_bids_then_add_asks5) {
    Level2Interface l2;
    l2.add_order(10, 50, 1);
    l2.add_order(10, 25, 1);
    l2.add_order(10, 72, 1);
    l2.add_order(110, 80, 0);
    l2.add_order(30, 90, 1);
    l2.add_order(40, 30, 1);
    l2.add_order(30, 90, 0);
    l2.add_order(40, 80, 1);
    ASSERT_EQ(l2.get_l2_size(), 0);
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test1, add_bids_then_add_asks6) {
    Level2Interface l2;
    l2.add_order(10, 50, 1);
    l2.add_order(10, 25, 1);
    l2.add_order(10, 72, 1);
    l2.add_order(2, 10, 0);
    l2.add_order(1, 5, 0);
    l2.store();

    Level2Interface l2_1;
    l2_1.load();
//    std::vector<std::pair<OfferID, Count>> test {{2, 10}};
//    ASSERT_EQ(l2.get_l2_size(), 27);
//    ASSERT_EQ(l2.get_offers_by_price(72), test);
    switch(print_type) {
        case 1: l2_1.print_level2_by_price(); break;
        case 2: l2_1.print_level2_by_idx(); break;
    }
}


TEST(Test2, close_order1) {
    Level2Interface l2;
    l2.add_order(10, 50, 1);
    ASSERT_TRUE(l2.close_order(5, 0));
    ASSERT_EQ(l2.get_l2_size(), 5);
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test2, close_order2) {
    Level2Interface l2;
    l2.add_order(10, 50, 1);
    ASSERT_EQ(l2.get_l2_size(), 10);
    ASSERT_FALSE(l2.close_order(15, 0));
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test2, close_order3) {
    Level2Interface l2;
    l2.add_order(10, 50, 1);
    ASSERT_TRUE(l2.close_order(5, 0));
    ASSERT_TRUE(l2.close_order(5, 0));
    ASSERT_FALSE(l2.close_order(5, 0));
    ASSERT_FALSE(l2.close_order(5, 5));
    ASSERT_EQ(l2.get_l2_size(), 0);
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test3, get_prices1) {
    Level2Interface l2;
    l2.add_order(10, 50, 1);
    std::vector<std::pair<OfferID, Count>> v = l2.get_offers_by_price(50);
    ASSERT_EQ(v.begin()->first, 0);
    ASSERT_EQ(v.begin()->second, 10);
    ASSERT_EQ(l2.get_l2_size(), 10);
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test3, get_prices2) {
    Level2Interface l2;
    std::vector<std::pair<OfferID, Count>> v = l2.get_offers_by_price(0);
    ASSERT_EQ(v.size(), 0);
    ASSERT_EQ(l2.get_l2_size(), 0);
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test4, get_ids1) {
    Level2Interface l2;
    l2.add_order(10, 50, 1);
    std::pair<OfferID, Count> v = l2.get_offers_by_id(0);
    ASSERT_EQ(v.first, 50);
    ASSERT_EQ(v.second, 10);
    ASSERT_EQ(l2.get_l2_size(), 10);
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test4, get_ids2) {
    Level2Interface l2;
    std::pair<OfferID, Count> v = l2.get_offers_by_id(0);
    ASSERT_EQ(v.first, 0);
    ASSERT_EQ(v.second, 0);
    ASSERT_EQ(l2.get_l2_size(), 0);
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test5, get_size1) {
    Level2Interface l2;
    l2.add_order(10, 50, 1);
    l2.add_order(15, 20, 1);
    ASSERT_EQ(l2.get_l2_size(), 25);
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test5, get_size2) {
    Level2Interface l2;
    ASSERT_EQ(l2.get_l2_size(), 0);
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}