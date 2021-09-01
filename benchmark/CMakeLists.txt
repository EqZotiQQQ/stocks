project(stocks_benchmarks)

message("benchmarks stocks...")

add_executable(stocks_benchmarks main.cpp)

include_directories(../stocks-stl/headers)
include_directories(../stocks-abseil-impl/headers)

target_link_libraries(stocks_benchmarks stocks-stl)
target_link_libraries(stocks_benchmarks stocks-absl-impl)

target_link_libraries(stocks_benchmarks benchmark::benchmark)
