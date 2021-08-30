#include "benchmark/benchmark.h"

#include "OrderBook.h"


void add_asks(benchmark::State& state)
{
    OrderBook l2;
    while (state.KeepRunning()) {
        for (int i = 0; i < 20000; i++) {
            Price price = rand() % 3000 + 50;
            Price quantity = rand() % 1000 + 40;
            l2.add_order(OFFER::ASK, price, quantity);
        }
    }
}

// faced an issue with big number of iterations... it works for gtests but fails for benchmark
//  kNanosecond, kMicrosecond, kMillisecond, kSecond
BENCHMARK(add_asks)->Arg(0)->Unit(benchmark::kNanosecond);

BENCHMARK_MAIN();