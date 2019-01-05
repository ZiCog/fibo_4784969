// 
// Google benchmark for fibo
//
// Compile with: g++ -O2 -o bench bench.cpp fibo.cpp -lbenchmark -lpthread
//
#include <benchmark/benchmark.h>

#include "bint.h"
#include "fibo.h"

bool fiboInitialized = false;

// Define fibo benchmark
static void BM_fibo(benchmark::State& state) {
	int n = state.range(0);
	for (auto _ : state) {
		if (!fiboInitialized) {
	      	fiboInit();
			fiboInitialized = true;
		}
	      benchmark::DoNotOptimize(fibo(n));
	}
}

BENCHMARK(BM_fibo)->Arg(4784969)->Unit(benchmark::kMillisecond);

BENCHMARK(BM_fibo)->RangeMultiplier(2)->RangeMultiplier(2)->Range(1<<4, 1<<25)->Complexity()->Unit(benchmark::kMillisecond);

// Define fibo benchmark
static void BM_fiboEjOlson(benchmark::State& state) {
	int n = state.range(0);
	for (auto _ : state) {
            fiboEjOlson(n);
	}
}

BENCHMARK(BM_fiboEjOlson)->Arg(4784969)->Unit(benchmark::kMillisecond);

BENCHMARK(BM_fiboEjOlson)->RangeMultiplier(2)->RangeMultiplier(2)->Range(1<<4, 1<<25)->Complexity()->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
