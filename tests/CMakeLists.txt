project(stocks_tests)

add_executable(stocks_tests main.cpp)

include_directories(../stocks/headers)

target_link_libraries(stocks_tests stocks)
target_link_libraries(stocks_tests gtest gtest_main)
