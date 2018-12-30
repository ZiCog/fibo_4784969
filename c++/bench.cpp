// 
// Google benchmark for fibo
//
// Compile with: g++ -O2 -o bench bench.cpp -lbenchmark -lpthread
//
#include <benchmark/benchmark.h>
#include <unordered_map>

#include "bint.h"

const bint zero = "0";
const bint one = "1";
const bint two = "2";

std::unordered_map<uint64_t, bint> memo;

int isEven(int n) { return (n & 1) == 0; }

// This Fibonacci version derived from Paeryn's Haskell example.
const bint fibo (int n) {
    if (memo.find(n) != memo.end()) {
        return memo[n];    
    }

    int k = (n / 2);
    const bint a = fibo(k);
    const bint b = fibo(k - 1);
    if (isEven(n)) {
        return memo[n] = a * (two * b + a);
    }
    if ((n % 4) == 1) {
        return memo[n] = (two * a + b) * (two * a - b) + two;
    }
    return memo[n] = (two * a + b) * (two * a - b) - two;
}

// Define fibo benchmark
static void BM_fibo(benchmark::State& state) {
	bint res;

	// Initialize the fibo's memo.
	memo.clear();
	memo[0] = zero;
	memo[1] = one;
	memo[2] = one;

	for (auto _ : state)
		for (int i = 0; i < 1000; i++) {
			res = fibo(4784969);
		}
}
BENCHMARK(BM_fibo)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
