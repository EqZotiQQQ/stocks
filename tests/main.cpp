#include "level2.h"

#include "gtest/gtest.h"

TEST(Test1, createLevel2) {
    std::cout << "Hello tests\n";
    ASSERT_EQ(1,1);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}