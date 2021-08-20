cmake_minimum_required(VERSION 3.16)
project(stocks)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)   # include additional tools
include(utils)  # execute this .cmake. It contains google tests.

add_subdirectory(stocks)

# google testts
add_subdirectory(tests)