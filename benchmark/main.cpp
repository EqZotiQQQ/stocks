#include "../stocks-abseil-impl/headers/OrderBook.h"
#include "../stocks-stl/headers/OrderBook.h"

#include "benchmark/benchmark.h"

using namespace stl;

void add_asks_stl(benchmark::State& state)
{
    OrderBook l2;
    while (state.KeepRunning()) {
        for (int i = 0; i < 2000000; i++) {
            Price price = rand() % 1 + 50;
            Price quantity = rand() % 1 + 40;
            l2.add_order(ORDER_TYPE::ASK, price, quantity);
        }
    }
}

void add_mixed_stl(benchmark::State& state)
{
    OrderBook l2;
    while (state.KeepRunning()) {
        for (int i = 0; i < 2000000; i++) {
            Price price = rand() % 1 + 50;
            Price quantity = rand() % 1 + 40;
            if (i % 2 == 0) {
                l2.add_order(ORDER_TYPE::ASK, price, quantity);
            } else {
                l2.add_order(ORDER_TYPE::BID, price, quantity);
            }
        }
    }
}
void create_then_close_orders_stl(benchmark::State& state)
{
    OrderBook l2;
    std::vector<OfferId> ids;
    std::vector<Qty> qtys;

    while (state.KeepRunning()) {
        for (int i = 0; i < 2000000; i++) {
            Price price = rand() % 1 + 50;
            Price quantity = rand() % 1 + 40;
            ids.push_back(i);
            qtys.push_back(quantity);
            l2.add_order(ORDER_TYPE::ASK, price, quantity);
        }

        for (int i = 0; i < ids.size(); i++) {
            l2.close_order(ids[i], qtys[i]);
        }
    }
}

void store_stl(benchmark::State& state)
{
    OrderBook l2;
    std::vector<OfferId> ids;
    std::vector<Qty> qtys;

    while (state.KeepRunning()) {
        for (int i = 0; i < 2000000; i++) {
            Price price = rand() % 1 + 50;
            Price quantity = rand() % 1 + 40;
            ids.push_back(i);
            qtys.push_back(quantity);
            l2.add_order(ORDER_TYPE::ASK, price, quantity);
        }
        l2.store();
    }
}

void store_then_load_stl(benchmark::State& state)
{
    OrderBook l2;
    std::vector<OfferId> ids;
    std::vector<Qty> qtys;

    while (state.KeepRunning()) {
        for (int i = 0; i < 2000000; i++) {
            Price price = rand() % 1 + 50;
            Price quantity = rand() % 1 + 40;
            ids.push_back(i);
            qtys.push_back(quantity);
            l2.add_order(ORDER_TYPE::ASK, price, quantity);
        }
        l2.store();
        l2.load();
    }
}

void add_asks_abseil(benchmark::State& state)
{
    OrderBookAbseil l2;
    while (state.KeepRunning()) {
        for (int i = 0; i < 2000000; i++) {
            Price price = rand() % 1 + 50;
            Price quantity = rand() % 1 + 40;
            l2.add_order(Offer::ASK, price, quantity);
        }
    }
}

// faced an issue with big number of iterations... it works for gtests but fails for benchmark
//  kNanosecond, kMicrosecond, kMillisecond, kSecond
//BENCHMARK(add_asks_stl)->Arg(0)->Unit(benchmark::kMillisecond);
//BENCHMARK(add_mixed_stl)->Arg(0)->Unit(benchmark::kMillisecond);
//BENCHMARK(add_asks_abseil)->Arg(0)->Unit(benchmark::kMillisecond);

BENCHMARK(add_asks_stl)->Arg(0)->Unit(benchmark::kMillisecond);
BENCHMARK(add_mixed_stl)->Arg(0)->Unit(benchmark::kMillisecond);
BENCHMARK(create_then_close_orders_stl)->Arg(0)->Unit(benchmark::kMillisecond);
BENCHMARK(store_stl)->Arg(0)->Unit(benchmark::kMillisecond);
BENCHMARK(store_then_load_stl)->Arg(0)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();