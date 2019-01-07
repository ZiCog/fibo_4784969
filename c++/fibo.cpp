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
        bint t1 = b + b;
        bint t2 = t1 - a;
        t1 = a * t2;
        bint t3 = b * t2;
        if (n % 4 == 0) {
            t3 = t3 - one;       // FIXME:  t3 - 1 silently fails. Why?
        } else {
            t3 = t3 + one;
        }
        a = t1;
        b = t3;
    } else {
        bint t1 = a + a;
        bint t2 = t1 + b;
        t1 = b * t2;
        bint t3 = a * t2;
        if ((n % 4) == 1) {
            t3 = t3 + one;
        } else {
            t3 = t3 - one;
        }
        a = t3;
        b = t1;
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
    static bint a;
    fiboNewWork((n - 1) / 2, a, b);
    if (n % 2 == 0) {
        // b=b*(2*a+b)
        bint t1 = a + a;
        bint t2 = t1 + b;
        t1 = b * t2;
        b = t1;
    } else {
        // b=b*(2*b-a)-(-1)^k
        bint t1 = b + b;
        bint t2 = t1 - a;
        bint t3 = b * t2;
        if (n % 4 == 1) {
            t3 = t3 - one;
        } else {
            t3 = t3 + one;
        }
        b = t3;
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

