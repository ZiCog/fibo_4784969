// 
// Google benchmark for threaded multiplication
//
// Compile with: g++ -O2 -o thread threads.cpp fibo.cpp -lbenchmark -lpthread
//
#include <benchmark/benchmark.h>
#include <iostream>
#include <thread>
#include <future>


#include "bint.h"

bint res1;
bint res2;

bint power(int n)
{
	bint x = bint("1");
	bint ten = bint("10");
	for (int i = 0; i < n; i++) { 
		x = x * ten;
	}
	return x;
}

#define N 10

// Define threaded mul benchmark
static void BM_mulThreaded(benchmark::State& state) {
	int n = state.range(0);
	for (auto _ : state) {
		auto future1 = std::async([n](){return power(n);});
		auto future2 = std::async([n](){return power(n);});
		benchmark::DoNotOptimize(res1 = future1.get());
		benchmark::DoNotOptimize(res2 = future2.get());
	}
}
BENCHMARK(BM_mulThreaded)->RangeMultiplier(2)->Range(1<<4, 1<<16)->Unit(benchmark::kMillisecond);

// Define mul benchmark
static void BM_mul(benchmark::State& state) {
	int n = state.range(0);
	for (auto _ : state) {
		benchmark::DoNotOptimize(res1 = power(n));
		benchmark::DoNotOptimize(res2 = power(n));
	}
}
BENCHMARK(BM_mul)->RangeMultiplier(2)->Range(1<<4, 1<<16)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
