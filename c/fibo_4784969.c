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

    printf("%f\n", elapsedTime);
}

int main( void )
{
//    timeIt(4784969);

    for (int n = 10; n <= 10000000; n *= 10) {
        timeIt(n);
    }

    return (0);
}

