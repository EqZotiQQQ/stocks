cmake_minimum_required(VERSION 3.16)

project(stocks-stl)

set(CMAKE_CXX_STANDARD 14)

set(ABSL_PROPAGATE_CXX_STD ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)   # include additional tools
include(gtests)  # execute this .cmake. It contains google tests.
include(benchmark)  # execute this .cmake. It contains google tests.
include(json)  # execute this .cmake. It contains nlohmann_json.
include(cppcheck)
include(clang-format)



add_subdirectory(stocks-abseil-impl)
add_subdirectory(stocks-stl)



# google tests
add_subdirectory(tests)
# google benchmarks
add_subdirectory(benchmark)
# google stl analog
add_subdirectory(3rdparty/abseil-cpp)