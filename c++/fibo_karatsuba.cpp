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
    std::cout << res << std::endl;
    return 0;
}
