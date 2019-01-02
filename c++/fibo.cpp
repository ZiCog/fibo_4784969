#include <unordered_map>

#include "bint.h"

const bint zero = "0";
const bint one = "1";
const bint two = "2";

std::unordered_map<uint64_t, bint> memo;

inline int isEven(int n) { return (n & 1) == 0; }

// This Fibonacci version derived from ejolson's doubling formula C example.
bint a;
bint b;
void fiboEjOlson(const int n) {
    if (n == 0) {
        a = zero;
        b = one;
        return;
    }
    fiboEjOlson(n / 2);
    bint ta = a;
    if (isEven(n)) {
        a = ta * ((b + b) - ta);
        b = (ta * ta) + (b * b);
    } else {
        a = (a * a) + (b * b);
        b = b * ((ta + ta) + b);
    }
}

void fiboInit() {
    // Initialize the fibo's memo.
    memo.clear();
    memo[0] = zero;
    memo[1] = one;
    memo[2] = one;
}

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
    bint twoa = two * a;
    if ((n % 4) == 1) {
        return memo[n] = (twoa + b) * (twoa - b) + two;
    }
    return memo[n] = (twoa + b) * (twoa - b) - two;
}

