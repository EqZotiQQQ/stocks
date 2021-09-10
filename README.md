Simple orderbook written on C++14. Here is 2 implementations, using stl and abseil.


build with:
cmake . -B cmake-build-debug
cmake --build cmake-build-debug -- -j16

tests list:
./stock_tests --gtest_list_tests

launch specific testcase:
./stock_tests --gtest_filter=<testcase_name>

cmake commands that executes on build:
cmake . -B cmake-build-debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON 
