cmake_minimum_required(VERSION 3.16)
message("stocks...")
project(stocks)

set(HEADERS headers/level2.h)

set(SOURCES src/level2.cpp)

include_directories(
        headers
)

add_library(stocks STATIC ${HEADERS} ${SOURCES})