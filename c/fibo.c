#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>

void fibo(int n) {
    mpz_t res;
    mpz_init(res);

    mpz_fib_ui(res, n);

    gmp_printf( "%Zd\n", res );
}

int main(int argc, char* argv[] )
{
    int n = 4784969;   // The first Fibonacci number with a million digits

    if (argc >= 2) {
        n = atol(argv[1]);
    }

    fibo(n);
    return (0);
}

