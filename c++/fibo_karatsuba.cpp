#include "bint.h"
#include <time.h>

bint zero = "0";
bint one = "1";
bint two = "2";

bint memo[] = {zero, one, one};

int isEven(int n) { return (n & 1) == 0; }

bint fibo(int n) {
    if (n <= 2) {
        return memo[n];
    }
    int k = (n / 2);
    bint fk = fibo(k);
    bint fk1 = fibo(k + 1);
    if (isEven(n)) {
        return fk * (fk1 * two - fk);
    }
    return (fk * fk) + (fk1 * fk1);
}

int main(int argc, char *argv[]) {
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
    std::cout << "allocBytes: " << allocBytes << std::endl;
#endif

    return 0;
}
