// 
// Google benchmark for threaded multiplication
//
// Compile with: g++-6 -Wall -Wextra -DUSE_OMP -fopenmp -O2 -o threads threads.cpp -lbenchmark -lpthread
//
#include <benchmark/benchmark.h>
#include <iostream>
#include <thread>
#include <future>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "bint.h"

bint res1;
bint res2;
bint res3;
bint res4;
bint res5;
bint res6;
bint res7;
bint res8;

bint power(int n)
{
	bint x = bint("1");
	bint ten = bint("10");
	for (int i = 0; i < n; i++) { 
		x = x * ten;
	}
	return x;
}

// Define OpenMP threaded power() benchmark
static void BM_powerOmp(benchmark::State& state) {
	int n = state.range(0);
	#pragma omp parallel
	{
		#pragma omp single
		{
			for (auto _ : state) {
				#pragma omp task default(shared)
				benchmark::DoNotOptimize(res1 = power(n));

				#pragma omp task default(shared)
				benchmark::DoNotOptimize(res2 = power(n));

				#pragma omp task default(shared)
				benchmark::DoNotOptimize(res3 = power(n));

				benchmark::DoNotOptimize(res4 = power(n));

				#pragma omp taskwait
			}
		}
	}
}
BENCHMARK(BM_powerOmp)->RangeMultiplier(2)->Range(1<<4, 1<<14)->Unit(benchmark::kMillisecond);

// Define std::async threaded power() benchmark
static void BM_powerAsync(benchmark::State& state) {
	int n = state.range(0);
	for (auto _ : state) {
		auto future1 = std::async([n](){return power(n);});
		auto future2 = std::async([n](){return power(n);});
		auto future3 = std::async([n](){return power(n);});
		res4 = power(n);
		benchmark::DoNotOptimize(res1 = future1.get());
		benchmark::DoNotOptimize(res2 = future2.get());
		benchmark::DoNotOptimize(res3 = future3.get());
	}
}
BENCHMARK(BM_powerAsync)->RangeMultiplier(2)->Range(1<<4, 1<<14)->Unit(benchmark::kMillisecond);

// Define serial() power benchmark
static void BM_powerSer(benchmark::State& state) {
	int n = state.range(0);
	for (auto _ : state) {
		benchmark::DoNotOptimize(res1 = power(n));
		benchmark::DoNotOptimize(res2 = power(n));
		benchmark::DoNotOptimize(res3 = power(n));
		benchmark::DoNotOptimize(res4 = power(n));
	}
}
BENCHMARK(BM_powerSer)->RangeMultiplier(2)->Range(1<<4, 1<<14)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
