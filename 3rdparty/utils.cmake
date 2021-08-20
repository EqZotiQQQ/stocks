cmake_minimum_required(VERSION 3.19)

message(STATUS "FetchContent: googletest")

include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG master
)
FetchContent_MakeAvailable(googletest)