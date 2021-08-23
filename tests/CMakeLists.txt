project(stock_tests)

message("test stocks...")

add_executable(stock_tests main.cpp)

include_directories(../stocks/headers)

target_link_libraries(stock_tests stocks)
#target_link_libraries(stock_tests gtest gtest_main)