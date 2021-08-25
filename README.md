Собрать уровень стакан можно при помощи:

cmake . -B cmake-build-debug

cmake --build cmake-build-debug -- -j16

launch specific testcase
./stock_tests --gtest_filter=<testcase_name>

get all test cases
./stock_tests --gtest_list_tests

to see the commands list that will be executed
cmake . -B cmake-build-debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON 
