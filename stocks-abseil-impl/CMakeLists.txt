cmake_minimum_required(VERSION 3.16)

project(stocks)

set(HEADERS headers/OrderBook.h)

set(SOURCES src/OrderBook.cpp)

include_directories(
        headers
)

add_library(stocks STATIC ${HEADERS} ${SOURCES})

target_link_libraries(stocks PUBLIC nlohmann_json::nlohmann_json)

target_link_libraries(stocks PUBLIC
        absl::flat_hash_map
        absl::strings
        absl::str_format_internal
        )