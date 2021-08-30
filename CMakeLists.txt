cmake_minimum_required(VERSION 3.16)

project(stocks)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)   # include additional tools
include(gtests)  # execute this .cmake. It contains google tests.
include(benchmark)  # execute this .cmake. It contains google tests.
include(json)  # execute this .cmake. It contains nlohmann_json.
include(cppcheck)
include(clang-format)

add_subdirectory(stocks)



# google tests
add_subdirectory(tests)
# google benchmarks
add_subdirectory(benchmark)