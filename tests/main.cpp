#include "level2Interface.h"

#include "gtest/gtest.h"

#include <iostream>

#define print std::cout << __FUNCTION__ <<std::endl;
TEST(Test1, create_level2_and_add_some_orders_with_same_prices) {
    Level2Interface l2;
    l2.add_order(4, 50, 1);
    l2.add_order(2, 25, 1);
    l2.add_order(5, 72, 1);
    l2.add_order(1, 72, 1);
    l2.print_level2_by_price();
//    l2.print_level2_by_idx();
}
TEST(Test1, add_bids_then_add_asks1) {
    Level2Interface l2;
    l2.add_order(10, 50, 1);
    l2.add_order(10, 25, 1);
    l2.add_order(10, 72, 1);
    l2.add_order(2, 35, 0);
    l2.add_order(1, 65, 0);
    l2.print_level2_by_price();
//    l2.print_level2_by_idx();
}
TEST(Test1, add_bids_then_add_asks2) {
    Level2Interface l2;
    l2.add_order(10, 50, 1);
    l2.add_order(10, 25, 1);
    l2.add_order(10, 72, 1);
    l2.add_order(11, 35, 0);
    l2.print_level2_by_price();
//    l2.print_level2_by_idx();
}
TEST(Test1, add_bids_then_add_asks3) {
    Level2Interface l2;
    l2.add_order(10, 50, 1);
    l2.add_order(10, 25, 1);
    l2.add_order(10, 72, 1);
    l2.add_order(11, 65, 0);
    l2.print_level2_by_price();
//    l2.print_level2_by_idx();
}
TEST(Test1, add_bids_then_add_asks4) {
    Level2Interface l2;
    l2.add_order(10, 50, 1);
    l2.add_order(10, 25, 1);
    l2.add_order(10, 72, 1);
    l2.add_order(110, 80, 0);
    l2.print_level2_by_price();
//    l2.print_level2_by_idx();
}
TEST(Test1, add_bids_then_add_asks5) {
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
    l2.print_level2_by_price();
//    l2.print_level2_by_idx();
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

