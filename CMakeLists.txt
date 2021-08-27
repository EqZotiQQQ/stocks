cmake_minimum_required(VERSION 3.16)

project(stocks)

set(CMAKE_CXX_STANDARD 14)
#set(BOOST_ENABLE_CMAKE ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)   # include additional tools
include(gtests)  # execute this .cmake. It contains google tests.
include(json)  # execute this .cmake. It contains google tests.

#include(boost)  # execute this .cmake. It contains google tests.

add_subdirectory(stocks)

# google testts
add_subdirectory(tests)