#include "level2.h"

#include "gtest/gtest.h"

#define print ;//std::cout << __FILE__ << ":" << __LINE__ << '\t' << __FUNCTION__ << '\n';

TEST(Test1, createLevel2) {
    Level2 l2;
    ASSERT_EQ(l2.get_l2_size(), 0);
}

TEST(Test1, add_one_offer) {
    Level2 l2;
    unsigned long long order_id = l2.add_order(3, 223123, 1);
    ASSERT_EQ(order_id,1);
    ASSERT_EQ(l2.get_l2_size(), 3);
}

TEST(Test1, add_few_offers) {
    Level2 l2;
    unsigned long long order_id1 = l2.add_order(3, 223123, 1);
    unsigned long long order_id2 = l2.add_order(5, 1223123, 1);
    unsigned long long order_id3 = l2.add_order(1, 4221, 1);
    ASSERT_EQ(order_id1,1);
    ASSERT_EQ(order_id2,2);
    ASSERT_EQ(order_id3,3);
    ASSERT_EQ(l2.get_l2_size(), 9);
}

TEST(Test1, remove_one) {
    Level2 l2;
    ASSERT_EQ(l2.get_l2_size(), 0);
    ASSERT_FALSE(l2.close_order(5, 0));
}

TEST(Test1, add_then_remove_one) {
    Level2 l2;
    unsigned long long order_id1 = l2.add_order(3, 223123, 1);
    unsigned long long order_id2 = l2.add_order(5, 1223123, 1);
    unsigned long long order_id3 = l2.add_order(1, 4221, 1);
    ASSERT_EQ(l2.get_l2_size(), 9);
    ASSERT_TRUE(l2.close_order(5, order_id2));
    ASSERT_EQ(l2.get_l2_size(), 4);
}

TEST(Test1, add_then_remove_all_offer) {
    Level2 l2;
    unsigned long long order_id1 = l2.add_order(3, 223123, 1);
    unsigned long long order_id2 = l2.add_order(5, 1223123, 1);
    unsigned long long order_id3 = l2.add_order(1, 4221, 1);
    ASSERT_TRUE(l2.close_order(1, order_id2));
    ASSERT_EQ(l2.get_l2_size(), 8);
}

TEST(Test1, add_then_remove_more_then_available) {
    Level2 l2;
    unsigned long long order_id1 = l2.add_order(3, 223123, 1);
    unsigned long long order_id2 = l2.add_order(5, 1223123, 1);
    unsigned long long order_id3 = l2.add_order(1, 4221, 1);
    ASSERT_FALSE(l2.close_order(12, order_id2));
    ASSERT_EQ(l2.get_l2_size(), 9);
}

TEST(TEST2, add_ask) {
    Level2 l2;
    l2.add_order(5, 223, 0);
}



int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}