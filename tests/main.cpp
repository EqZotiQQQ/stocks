#include "level2Interface.h"

#include "gtest/gtest.h"

#include <iostream>

constexpr int print_type{1};

TEST(Test1, create_level2_and_add_some_orders_with_same_prices) {
    Level2Interface l2;
    l2.add_order(4, 50, OFFER::BID);
    l2.add_order(2, 25, OFFER::BID);
    l2.add_order(5, 72, OFFER::BID);
    l2.add_order(1, 72, OFFER::BID);

    std::vector<OfferById> test {{1, 2}};
    std::vector<OfferById> subject = l2.get_offers_by_price(25);

    ASSERT_EQ(l2.get_l2_size(), 12);
    ASSERT_EQ(subject.size(), test.size());
    for (int i = 0; i < test.size(); i++) {
        ASSERT_EQ(subject[i].offerId, test[i].offerId);
        ASSERT_EQ(subject[i].quantity, test[i].quantity);
    }
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test1, add_bids_then_add_asks1) {
    Level2Interface l2;
    l2.add_order(10, 50, OFFER::BID); // goes to 7
    l2.add_order(10, 25, OFFER::BID); // goes to 0
    l2.add_order(10, 72, OFFER::BID); // goes to 10

    l2.add_order(2, 35, OFFER::ASK);
    l2.add_order(1, 50, OFFER::ASK);
    l2.add_order(10, 65, OFFER::ASK);


    std::vector<OfferById> test {{2, 10}};
    std::vector<OfferById> subject = l2.get_offers_by_price(72);

    ASSERT_EQ(l2.get_l2_size(), 17);
    ASSERT_EQ(subject.size(), test.size());
    for (int i = 0; i < test.size(); i++) {
        ASSERT_EQ(subject[i].offerId, test[i].offerId);
        ASSERT_EQ(subject[i].quantity, test[i].quantity);
    }


    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test1, add_bids_then_add_asks2) {
    Level2Interface l2;
    l2.add_order(10, 50, OFFER::BID);
    l2.add_order(10, 25, OFFER::BID);
    l2.add_order(10, 72, OFFER::BID);
    l2.add_order(11, 35, OFFER::ASK);


    std::vector<OfferById> test {{2, 10}};


   // ASSERT_EQ(l2.get_offers_by_price(72), test);
    std::vector<OfferById> test2 {{3, 1}};
   // ASSERT_EQ(l2.get_offers_by_price(35), test2);
   // ASSERT_EQ(l2.get_l2_size(), 21);
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test1, add_bids_then_add_asks3) {
    Level2Interface l2;
    l2.add_order(10, 50, OFFER::BID);
    l2.add_order(10, 25, OFFER::BID);
    l2.add_order(10, 72, OFFER::BID);
    l2.add_order(11, 65, OFFER::ASK);
    std::vector<OfferById> test {{0, 9}};
    //ASSERT_EQ(l2.get_l2_size(), 19);
    //ASSERT_EQ(l2.get_offers_by_price(50), test);
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test1, add_bids_then_add_asks4) {
    Level2Interface l2;
    l2.add_order(10, 50, OFFER::BID);
    l2.add_order(10, 25, OFFER::BID);
    l2.add_order(10, 72, OFFER::BID);
    l2.add_order(110, 80, OFFER::ASK);
    //ASSERT_EQ(l2.get_l2_size(), 80);
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test1, add_bids_then_add_asks5) {
    Level2Interface l2;
    l2.add_order(10, 50, OFFER::BID);
    l2.add_order(10, 25, OFFER::BID);
    l2.add_order(10, 72, OFFER::BID);
    l2.add_order(110, 80, OFFER::ASK);
    l2.add_order(30, 90, OFFER::BID);
    l2.add_order(40, 30, OFFER::BID);
    l2.add_order(30, 90, OFFER::ASK);
    l2.add_order(40, 80, OFFER::BID);
   // ASSERT_EQ(l2.get_l2_size(), 0);
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test1, add_bids_then_add_asks6) {
    Level2Interface l2;
    l2.add_order(10, 50, OFFER::BID);
    l2.add_order(10, 25, OFFER::BID);
    l2.add_order(10, 72, OFFER::BID);
    l2.add_order(2, 10, OFFER::ASK);
    l2.add_order(1, 5, OFFER::ASK);
    l2.store();

    Level2Interface l2_1;
    l2_1.load();
    std::vector<OfferById> test {{2, 10}};
  // ASSERT_EQ(l2.get_l2_size(), 33);
  // ASSERT_EQ(l2.get_offers_by_price(72), test);
    switch(print_type) {
        case 1: l2_1.print_level2_by_price(); break;
        case 2: l2_1.print_level2_by_idx(); break;
    }
}

TEST(Test1, add_bids_then_add_asks7) {
    Level2Interface l2;
    l2.add_order(10, 50, OFFER::BID);
    OfferID id1 = l2.add_order(10, 25, OFFER::BID);
    l2.add_order(10, 72, OFFER::BID);
    l2.add_order(10, 150, OFFER::BID);
    l2.add_order(110, 80, OFFER::ASK);


    std::vector<OfferById>* v = nullptr;
    std::vector<OfferById> vector_tst{{4, 80}};

    l2.get_offers_by_price(80, v);
    std::vector<OfferById> t2 = l2.get_offers_by_price(80);

    for (int i = 0; i < v->size(); i++) {
   //     ASSERT_EQ(vector_tst[i].offerId, v->at(i).offerId);
   //     ASSERT_EQ(v->at(i).offerId, t2.at(i).offerId);
    }


    OfferByPrice* pair_tst = nullptr;

    l2.get_offers_by_id(id1, pair_tst);
    auto test_pair = make_pair(25,10);
 //   ASSERT_EQ(pair_tst->quantity, test_pair.first);

    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test2, close_order1) {
    Level2Interface l2;
    l2.add_order(10, 50, OFFER::BID);
   // ASSERT_TRUE(l2.close_order(5, 0));
   // ASSERT_EQ(l2.get_l2_size(), 5);
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test2, close_order2) {
    Level2Interface l2;
    l2.add_order(10, 50, OFFER::BID);
 //   ASSERT_EQ(l2.get_l2_size(), 10);
 //   ASSERT_FALSE(l2.close_order(15, 0));
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test2, close_order3) {
    Level2Interface l2;
    l2.add_order(10, 50, OFFER::BID);
   // ASSERT_TRUE(l2.close_order(5, 0));
   // ASSERT_TRUE(l2.close_order(5, 0));
   // ASSERT_FALSE(l2.close_order(5, 0));
   // ASSERT_FALSE(l2.close_order(5, 5));
   // ASSERT_EQ(l2.get_l2_size(), 0);
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test3, get_prices1) {
    Level2Interface l2;
    l2.add_order(10, 50, OFFER::BID);
    std::vector<OfferById> v = l2.get_offers_by_price(50);
  //  ASSERT_EQ(v.begin()->offerId, 0);
  //  ASSERT_EQ(v.begin()->quantity, 10);
  //  ASSERT_EQ(l2.get_l2_size(), 10);
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test3, get_prices2) {
    Level2Interface l2;
    std::vector<OfferById> v = l2.get_offers_by_price(0);
 //   ASSERT_EQ(v.size(), 0);
 //   ASSERT_EQ(l2.get_l2_size(), 0);
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test4, get_ids1) {
    Level2Interface l2;
    l2.add_order(10, 50, OFFER::BID);
    OfferByPrice v = l2.get_offers_by_id(0);
 //   ASSERT_EQ(v.price, 50);
 //   ASSERT_EQ(v.quantity, 10);
 //   ASSERT_EQ(l2.get_l2_size(), 10);
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test4, get_ids2) {
    Level2Interface l2;
    OfferByPrice v = l2.get_offers_by_id(0);
  //  ASSERT_EQ(v.price, 0);
  //  ASSERT_EQ(v.quantity, 0);
  //  ASSERT_EQ(l2.get_l2_size(), 0);
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test5, get_size1) {
    Level2Interface l2;
    l2.add_order(10, 50, OFFER::BID);
    l2.add_order(15, 20, OFFER::BID);
  //  ASSERT_EQ(l2.get_l2_size(), 25);
    switch(print_type) {
        case 1: l2.print_level2_by_price(); break;
        case 2: l2.print_level2_by_idx(); break;
    }
}

TEST(Test5, get_size2) {
    Level2Interface l2;
 //   ASSERT_EQ(l2.get_l2_size(), 0);
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