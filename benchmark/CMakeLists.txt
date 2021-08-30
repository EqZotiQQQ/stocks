project(stocks_benchmarks)

message("benchmarks stocks...")

add_executable(stocks_benchmarks main.cpp)

include_directories(../stocks-stl/headers)

target_link_libraries(stocks_benchmarks stocks)
target_link_libraries(stocks_benchmarks benchmark::benchmark)
