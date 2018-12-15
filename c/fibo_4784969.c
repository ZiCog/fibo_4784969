#include <stdio.h>
#include <gmp.h>
#include <time.h>


void timeIt(int n) {
    mpz_t res;
    mpz_init( res );

    double startTime;
    double endTime;
    double elapsedTime;

    startTime = (float)clock()/CLOCKS_PER_SEC;

    mpz_fib_ui( res, n );

    endTime = (float)clock()/CLOCKS_PER_SEC;
    elapsedTime = endTime - startTime;

//    gmp_printf( "%Zd\n", res );

    printf("%d, %f\n", n, elapsedTime);
}

int main( void )
{
//    timeIt(4784969);

    for (int n = 2; n <= 1024 * 1024 * 256; n *= 2) {
        timeIt(n);
    }

    return (0);
}

