#include <time.h>
#include <unordered_map>
#include <time.h>

#include "bint.h"

//using fint = bint<>;

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

bint timeIt(int n) {
    double endTime;
    double elapsedTime;
    double startTime = clock() / CLOCKS_PER_SEC;

    // Initialize the fibo's memo.
    memo.clear();
    memo[0] = zero;
    memo[1] = one;
    memo[2] = one;

    bint res = fibo(n);
//    fiboEjOlson(n - 1);
//    bint res = b;

    endTime = (float)clock()/CLOCKS_PER_SEC;
    elapsedTime = endTime - startTime;
#if DEBUG
//    std::cout << "Compute time: " << elapsedTime << std::endl;
#endif
    return res;
}

int main(int argc, char *argv[]) {
    bint res = timeIt(4784969);
    std::cout << res << std::endl;

#if DEBUG
    std::cout << "allocWithWidth: " << allocWithWidth << std::endl;
    std::cout << "allocCopy: " << allocCopy << std::endl;
    std::cout << "allocString: " << allocString << std::endl;
    std::cout << "allocEquals: " << allocEquals << std::endl;
    std::cout << "allocEqualString: " << allocEqualsString << std::endl;
    std::cout << "allocHigh: " << allocHigh << std::endl;
    std::cout << "allocLow: " << allocLow << std::endl;
    std::cout << "allocShift: " << allocShift << std::endl;
    std::cout << "allocGrow: " << allocGrow << std::endl;
    std::cout << "allocBytes: " << allocBytes << std::endl;
    std::cout << "allocations: " << allocations << std::endl;
    std::cout << "mulCount: " << mulCount << std::endl;

    for (int i = 0; i < 17; i++) {
        std::cout << allocs[i] << std::endl;
    }
#endif
    return 0;
}
