cmake_minimum_required(VERSION 3.16)
message("stocks...")
project(stocks)

set(HEADERS headers/level2HashTableBase.h headers/level2Interface.h headers/level2BinaryTreeBase.h headers/level2MultiIndexBase.h)

set(SOURCES src/level2HashTableBase.cpp src/level2BinaryTreeBase.cpp src/level2MultiIndexBase.cpp)

include_directories(
        headers
)

add_library(stocks STATIC ${HEADERS} ${SOURCES})

#include_directories(${boostorg_SOURCE_DIR})

target_link_libraries(stocks PUBLIC nlohmann_json::nlohmann_json)

#target_link_libraries(stocks PUBLIC Boost::multi_index)