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

bint simplefunc(int n)
{
	bint x = 1;
	bint ten = "10";
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
		auto future1 = std::async(simplefunc, n);
		auto future2 = std::async(simplefunc, n);
		res1 = future1.get();
		res2 = future2.get();
//			std::cout << res << '\n';
	}
}
BENCHMARK(BM_mulThreaded)->RangeMultiplier(2)->Range(1<<4, 1<<14)->Unit(benchmark::kMillisecond);

// Define mul benchmark
static void BM_mul(benchmark::State& state) {
	int n = state.range(0);
	for (auto _ : state) {
		res1 = simplefunc(n);
		res2 = simplefunc(n);
//			std::cout << res << '\n';
	}
}
BENCHMARK(BM_mul)->RangeMultiplier(2)->Range(1<<4, 1<<14)->Unit(benchmark::kMillisecond);


BENCHMARK_MAIN();
