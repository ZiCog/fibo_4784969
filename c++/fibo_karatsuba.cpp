#include <string.h>

#include "bint.h"
#include "fibo.h"

int main(int argc, char *argv[]) {
    int n = 4784969;   // The first Fibonacci number with a million digits
    bint res; 

    if (argc >= 2) {
        n = atol(argv[1]);
    }

    fiboInit();

    fiboNew(n, res);
    //res = fibo(n);

    std::cout << res << std::endl;

    return 0;
}
