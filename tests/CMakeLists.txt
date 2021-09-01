project(stocks_tests)

#add_executable(stocks_tests stl_stock_test.cpp)
add_executable(stocks_tests abseil_stock_test.cpp)

#include_directories(../stocks-stl/headers)
include_directories(../stocks-abseil-impl/headers)

target_link_libraries(stocks_tests stocks-absl-impl)
target_link_libraries(stocks_tests gtest gtest_main)
