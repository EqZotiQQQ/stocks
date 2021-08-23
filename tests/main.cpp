#include "level2Interface.h"

#include <iostream>

#define print std::cout << __FUNCTION__ <<std::endl;
void t1() {
    print
    Level2Interface l2;
    l2.add_order(4, 50, 1);
    l2.add_order(2, 25, 1);
    l2.add_order(5, 72, 1);
    l2.add_order(1, 72, 1);
//    l2.print_level2_by_price();
    l2.print_level2_by_idx();
}
void t2() {
    print
    Level2Interface l2;
    l2.add_order(10, 50, 1);
    l2.add_order(10, 25, 1);
    l2.add_order(10, 72, 1);
    l2.add_order(2, 35, 0);
    l2.add_order(1, 65, 0);
//    l2.print_level2_by_price();
    l2.print_level2_by_idx();
}
void t3() {
    print
    Level2Interface l2;
    l2.add_order(10, 50, 1);
    l2.add_order(10, 25, 1);
    l2.add_order(10, 72, 1);
    l2.add_order(11, 35, 0);
//    l2.print_level2_by_price();
    l2.print_level2_by_idx();
}
void t4() {
    print
    Level2Interface l2;
    l2.add_order(10, 50, 1);
    l2.add_order(10, 25, 1);
    l2.add_order(10, 72, 1);
    l2.add_order(11, 65, 0);
//    l2.print_level2_by_price();
    l2.print_level2_by_idx();
}
void t5() {
    print
    Level2Interface l2;
    l2.add_order(10, 50, 1);
    l2.add_order(10, 25, 1);
    l2.add_order(10, 72, 1);
    l2.add_order(110, 80, 0);
//    l2.print_level2_by_price();
    l2.print_level2_by_idx();
}
void t6() {
    print
    Level2Interface l2;
    l2.add_order(10, 50, 1);
    l2.add_order(10, 25, 1);
    l2.add_order(10, 72, 1);
    l2.add_order(110, 80, 0);
    l2.add_order(30, 90, 1);
    l2.add_order(40, 30, 1);
    l2.add_order(30, 90, 0);
    l2.add_order(40, 80, 1);
//    l2.print_level2_by_price();
    l2.print_level2_by_idx();
}

int main() {
//    t1();
//    t2();
    t3();
//    t4();
//    t5();
//    t6();

    return 0;
}

#if 0

#include "gtest/gtest.h"

#define print ;//std::cout << __FILE__ << ":" << __LINE__ << '\t' << __FUNCTION__ << '\n';

TEST(Test1, createLevel2) {
    Level2Interface l2;
    ASSERT_EQ(l2.get_l2_size(), 0);
}

TEST(Test1, add_one_offer) {
    Level2Interface l2;
    unsigned long long order_id = l2.add_order(3, 223123, 1);
    ASSERT_EQ(order_id,1);
    ASSERT_EQ(l2.get_l2_size(), 3);
}

TEST(Test1, add_few_offers) {
    Level2Interface l2;
    unsigned long long order_id1 = l2.add_order(3, 223123, 1);
    unsigned long long order_id2 = l2.add_order(5, 1223123, 1);
    unsigned long long order_id3 = l2.add_order(1, 4221, 1);
    ASSERT_EQ(order_id1,1);
    ASSERT_EQ(order_id2,2);
    ASSERT_EQ(order_id3,3);
    ASSERT_EQ(l2.get_l2_size(), 9);
}

TEST(Test1, remove_one_from_empty_l2) {
    Level2Interface l2;
    ASSERT_EQ(l2.get_l2_size(), 0);
    ASSERT_FALSE(l2.close_order(5, 0));
}

TEST(Test1, add_then_remove_all_offer) {
    Level2Interface l2;
    unsigned long long order_id1 = l2.add_order(3, 223123, 1);
    unsigned long long order_id2 = l2.add_order(5, 1223123, 1);
    unsigned long long order_id3 = l2.add_order(1, 4221, 1);
    ASSERT_EQ(l2.get_l2_size(), 9);
    ASSERT_TRUE(l2.close_order(5, order_id2));
    ASSERT_EQ(l2.get_l2_size(), 4);
}

TEST(Test1, add_then_remove_one) {
    Level2Interface l2;
    unsigned long long order_id1 = l2.add_order(3, 223123, 1);
    unsigned long long order_id2 = l2.add_order(5, 1223123, 1);
    unsigned long long order_id3 = l2.add_order(1, 4221, 1);
    ASSERT_TRUE(l2.close_order(1, order_id2));
    ASSERT_EQ(l2.get_l2_size(), 8);
}

TEST(Test1, add_then_remove_more_then_available) {
    Level2Interface l2;
    unsigned long long order_id1 = l2.add_order(3, 223123, 1);
    unsigned long long order_id2 = l2.add_order(5, 1223123, 1);
    unsigned long long order_id3 = l2.add_order(1, 4221, 1);
    ASSERT_FALSE(l2.close_order(12, order_id2));
    ASSERT_EQ(l2.get_l2_size(), 9);
}

TEST(Test1, add_multiple_offers_to_one_bucket) {
    Level2Interface l2;
    unsigned long long order_id1 = l2.add_order(3, 223123, 1);
    unsigned long long order_id2 = l2.add_order(5, 1223123, 1);
    unsigned long long order_id3 = l2.add_order(1, 4221, 1);
    unsigned long long order_id4 = l2.add_order(4, 223123, 1);
    ASSERT_EQ(l2.get_l2_size(), 13);
}

TEST(TEST2, add_ask) {
    Level2Interface l2;
    l2.add_order(5, 223, 0);
}



int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#endif
