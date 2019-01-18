#include "fibo.h"

#include <unordered_map>

#include "bint.h"

const bint zero = "0";
const bint one = "1";
const bint two = "2";

std::unordered_map<uint64_t, bint> memo;

inline int isEven(int n) { return (n & 1) == 0; }

// This Fibonacci version derived from ejolson's doubling formula C example.
void fiboEjOlson(const int n, bint& a, bint& b) {
    if (n == 0) {
        a = zero;
        b = one;
        return;
    }
    fiboEjOlson(n / 2, a, b);
    bint ta = a;
    if (isEven(n)) {
        a = ta * ((b + b) - ta);
        b = (ta * ta) + (b * b);
    } else {
        a = (a * a) + (b * b);
        b = b * ((ta + ta) + b);
    }
}

void fiboNewWork(int n, bint& a, bint& b) {
    if (n == 0) {
        a = zero;
        b = one;
        return;
    }
    fiboNewWork (n / 2, a, b);
    if (n % 2 == 0) {
        // [a, b] = [a*(2*b-a), b*(2*b-a)-(-1)^k]
        bint t = two * b - a;
        a = a * t;
        b = b * t;
        if (n % 4 == 0) {
            b = b - one;
        } else {
            b = b + one;
        }
    } else {
        // [a, b] = [a*(2*a+b)+(-1)^k, b*(2*a+b)]
        bint t = two * a + b;
        b = b * t;
        a = a * t;
        if ((n % 4) == 1) {
            a = a + one;
        } else {
            a = a - one;
        }
    }
    return;
}

// This Fibonacci version derived from ejolson's doubling formula FreeBASIC example.
void fiboNew(int n, bint& b) {
    if (n == 0) {
        b = bint("0");
        return;
    }
    if (n == 1) {
        b = bint("1");
        return;
    }
    bint a;
    fiboNewWork((n - 1) / 2, a, b);
    if (n % 2 == 0) {
        // b=b*(2*a+b)
        b = b * (a + a + b);
    } else {
        // b=b*(2*b-a)-(-1)^k
        bint t = b * (b + b - a);
        if (n % 4 == 1) {
            b = t - one;
        } else {
            b = t + one;
        }
    }
    return;
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
        return  a * (two * b + a);
    }
    bint twoa = two * a;
    if ((n % 4) == 1) {
        return  (twoa + b) * (twoa - b) + two;
    }
    return  (twoa + b) * (twoa - b) - two;
}

