#include "tools.h"

int main(int argc, char** argv) {
    run_stl_tests(argc, argv);
    run_absl_tests(argc, argv);
    return 0;
}