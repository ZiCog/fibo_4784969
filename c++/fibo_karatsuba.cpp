#include <time.h>
#include <unordered_map>
#include <time.h>
#include <string.h>
#include "bint.h"

const bint zero = "0";
const bint one = "1";
const bint two = "2";

std::unordered_map<uint64_t, bint> memo;

int isEven(int n) { return (n & 1) == 0; }

// This Fibonacci version derived from ejolson's doubling formula C example.
bint a;
bint b;
static void fiboEjOlson(const int n) {
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

int main(int argc, char *argv[]) {
    int n = 4784969;   // The first Fibonacci number with a million digits
    bint res; 

    if (argc >= 2) {
        n = atol(argv[1]);
    }

    // Initialize the fibo's memo.
    memo.clear();
    memo[0] = zero;
    memo[1] = one;
    memo[2] = one;

    res = fibo(n);

    std::cout << res << std::endl;

    return 0;
}
