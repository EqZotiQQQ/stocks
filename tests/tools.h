#pragma once

#include <gtest/gtest.h>

#include "../stocks-abseil-impl/headers/OrderBook.h"
#include "../stocks-stl/headers/OrderBook.h"

int run_stl_tests(int argc, char** argv);
int run_absl_tests(int argc, char** argv);