Можно запихнуть в CLion и он подхватит и собрать то что нужно.
В предыдущей версии я вам отправил версию с гитом, тут её не будет, т.к. там есть лишние вещи.
Напирмер, там я ещё попробовал abseil либу юзать (гугловый аналог stl).
Решил его не прикладывать, в общем, если надо - пишите - отдам)


Собрать стакан можно при помощи:

cmake . -B cmake-build-debug

cmake --build cmake-build-debug -- -j16

Запустить специфичный тесткейс:
./stock_tests --gtest_filter=<testcase_name>

Список тестов:
./stock_tests --gtest_list_tests

Сгенерить список команд:
cmake . -B cmake-build-debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON 
