cmake_minimum_required(VERSION 3.16)
message("stocks...")
project(stocks)

set(HEADERS headers/level2HashTableBase.h headers/level2Interface.h headers/level2BinaryTreeBase.h)

set(SOURCES src/level2HashTableBase.cpp src/level2BinaryTreeBase.cpp)

include_directories(
        headers
)

add_library(stocks STATIC ${HEADERS} ${SOURCES})

target_link_libraries(stocks PRIVATE nlohmann_json::nlohmann_json)