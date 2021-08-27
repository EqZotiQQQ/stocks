cmake_minimum_required(VERSION 3.16)

message(STATUS "FetchContent: boost")

project(boostFetch)

include(FetchContent)

set(CMAKE_CXX_STANDARD 14)

FetchContent_Declare(
        boostorg
        GIT_REPOSITORY https://github.com/boostorg/boost.git
        GIT_TAG        master
)

FetchContent_GetProperties(boostorg)
if (NOT boostorg_POPULATED)
    FetchContent_Populate(boostorg)
    add_subdirectory(${boostorg_SOURCE_DIR} ${boostorg_BINARY_DIR})
endif ()

include_directories(${boostorg_SOURCE_DIR})