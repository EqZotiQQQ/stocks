cmake_minimum_required(VERSION 3.16)

project(stocks-stl)

set(HEADERS headers/OrderBook.h)

set(SOURCES src/OrderBook.cpp)

include_directories(
        headers
)

add_library(stocks-stl STATIC ${HEADERS} ${SOURCES})

target_link_libraries(stocks-stl PUBLIC nlohmann_json::nlohmann_json)