#include "../stocks-abseil-impl/headers/OrderBook.h"
#include "../stocks-stl/headers/OrderBook.h"

#include "benchmark/benchmark.h"

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

void add_asks_abseil(benchmark::State& state)
{
    OrderBookAbseil l2;
    while (state.KeepRunning()) {
        for (int i = 0; i < 20000; i++) {
            Price price = rand() % 3000 + 50;
            Price quantity = rand() % 1000 + 40;
            l2.add_order(Offer::ASK, price, quantity);
        }
    }
}

// faced an issue with big number of iterations... it works for gtests but fails for benchmark
//  kNanosecond, kMicrosecond, kMillisecond, kSecond
BENCHMARK(add_asks)->Arg(0)->Unit(benchmark::kMillisecond);
BENCHMARK(add_asks_abseil)->Arg(0)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();