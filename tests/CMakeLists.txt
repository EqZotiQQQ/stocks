project(stocks_tests)

add_executable(stocks_tests main.cpp abseil_stock_test.cpp stl_stock_test.cpp tools.h)

include_directories(../stocks-stl/headers)
include_directories(../stocks-abseil-impl/headers)

target_link_libraries(stocks_tests stocks-absl-impl)
target_link_libraries(stocks_tests stocks-stl)
target_link_libraries(stocks_tests gtest gtest_main)
