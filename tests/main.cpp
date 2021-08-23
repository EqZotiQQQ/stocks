#include "level2.h"

#include "gtest/gtest.h"

#define print ;//std::cout << __FILE__ << ":" << __LINE__ << '\t' << __FUNCTION__ << '\n';

TEST(Test1, createLevel2) {
    print
    Level2 l2;
    ASSERT_EQ(l2.get_l2_size(), 0);
}

TEST(Test1, add_one_offer) {
    print
    Level2 l2;
    unsigned long long order_id = l2.add_order(3, 223123);
    ASSERT_EQ(order_id,1);
    ASSERT_EQ(l2.get_l2_size(), 3);
}

TEST(Test1, add_few_offers) {
    print
    Level2 l2;
    unsigned long long order_id1 = l2.add_order(3, 223123);
    unsigned long long order_id2 = l2.add_order(5, 1223123);
    unsigned long long order_id3 = l2.add_order(1, 4221);
    ASSERT_EQ(order_id1,1);
    ASSERT_EQ(order_id2,2);
    ASSERT_EQ(order_id3,3);
    ASSERT_EQ(l2.get_l2_size(), 9);
//    l2.print_level2_by_idx();
//    std::cout << "\n\n";
//    l2.print_level2_by_price();
}

TEST(Test1, add_then_remove_one) {
    print
    Level2 l2;
    unsigned long long order_id1 = l2.add_order(3, 223123);
    unsigned long long order_id2 = l2.add_order(5, 1223123);
    unsigned long long order_id3 = l2.add_order(1, 4221);
    ASSERT_EQ(order_id1,1);
    ASSERT_EQ(order_id2,2);
    ASSERT_EQ(order_id3,3);
    ASSERT_EQ(l2.get_l2_size(), 9);
//    printf("size = %llu\n", l2.get_l2_size());
    //l2.print_level2_by_idx();
    ASSERT_TRUE(l2.close_order(1, order_id2));
    //l2.print_level2_by_idx();
//    ASSERT_EQ(l2.get_l2_size(), 8);
//    printf("size = %llu\n", l2.get_l2_size());
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}