cmake_minimum_required(VERSION 3.19)

message(STATUS "FetchContent: benchmark")

include(FetchContent)

FetchContent_Declare(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG master
)
FetchContent_MakeAvailable(benchmark)