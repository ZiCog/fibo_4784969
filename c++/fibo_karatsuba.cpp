#include <time.h>
#include <unordered_map>

#include "bint.h"

bint zero = "0";
bint one = "1";
bint two = "2";

int isEven(int n) { return (n & 1) == 0; }

std::unordered_map<uint64_t, bint> memo;

bint fibo(int n) {
    if (memo.find(n) != memo.end()) {
        return memo[n];    
    }
    int k = (n / 2);
    bint fk = fibo(k);
    bint fk1 = fibo(k + 1);
    if (isEven(n)) {
        bint result = fk * (fk1 * two - fk); 
        memo[n] = result;
        return result;
    }
    bint result = (fk * fk) + (fk1 * fk1);
    memo[n] = result;
    return result; 
}

int main(int argc, char *argv[]) {

    memo[0] = zero;
    memo[1] = one;
    memo[2] = one;

    bint res = fibo(4784969);
    //bint res = fibo(20000);
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
#endif

    return 0;
}
