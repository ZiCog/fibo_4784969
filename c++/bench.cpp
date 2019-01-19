// 
// Google benchmark for fibo
//
// Compile with: g++ -O2 -fopenmp -DUSE_OMP -o bench bench.cpp fibo.cpp -lbenchmark -lpthread
//
#include <benchmark/benchmark.h>

#include "bint.h"
#include "fibo.h"

#define MAX_POW_2 22

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

//BENCHMARK(BM_fibo)->Arg(4784969)->Unit(benchmark::kMillisecond);
//BENCHMARK(BM_fibo)->RangeMultiplier(2)->Range(1<<4, 1<<MAX_POW_2)->Unit(benchmark::kMillisecond);

/*
// Define fibo benchmark
static void BM_fiboEjOlson(benchmark::State& state) {
	int n = state.range(0);
	for (auto _ : state) {
            fiboEjOlson(n, a, b);
	}
}
*/
//BENCHMARK(BM_fiboEjOlson)->Arg(4784969)->Unit(benchmark::kMillisecond);
//BENCHMARK(BM_fiboEjOlson)->RangeMultiplier(2)->Range(1<<4, 1<<MAX_POW_2)->Unit(benchmark::kMillisecond);


// Define fibo benchmark
static void BM_fiboEjOlsonNewSer(benchmark::State& state) {
	int n = state.range(0);
	for (auto _ : state) {
            bint a;
            bint b;
		
	    fiboNew(n, b);
	}
}

BENCHMARK(BM_fiboEjOlsonNewSer)->Arg(4784969)->Unit(benchmark::kMillisecond);
//BENCHMARK(BM_fiboEjOlsonNew)->RangeMultiplier(2)->Range(1<<4, 1<<MAX_POW_2)->Unit(benchmark::kMillisecond);


// Define fibo benchmark
static void BM_fiboEjOlsonNewOmp(benchmark::State& state) {
	int n = state.range(0);
	for (auto _ : state) {
            #pragma omp parallel
	    {
		#pragma omp single
		{
                    bint a;
                    bint b;
		
	            fiboNew(n, b);
		}
	    }
	}
}

BENCHMARK(BM_fiboEjOlsonNewOmp)->Arg(4784969)->Unit(benchmark::kMillisecond);
//BENCHMARK(BM_fiboEjOlsonNew)->RangeMultiplier(2)->Range(1<<4, 1<<MAX_POW_2)->Unit(benchmark::kMillisecond);


BENCHMARK_MAIN();
