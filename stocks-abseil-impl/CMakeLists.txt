cmake_minimum_required(VERSION 3.16)

project(stocks-absl-impl)

set(HEADERS headers/OrderBook.h)

set(SOURCES src/OrderBook.cpp)

include_directories(
        headers
)

add_library(stocks-absl-impl STATIC ${HEADERS} ${SOURCES})

target_link_libraries(stocks-absl-impl PUBLIC nlohmann_json::nlohmann_json)

target_link_libraries(stocks-absl-impl PUBLIC
        absl::flat_hash_map
        absl::strings
        absl::str_format_internal
        )