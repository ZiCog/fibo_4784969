#include <stdio.h>
#include <gmp.h>
#include <time.h>

int main( void )
{
    mpz_t res;
    mpz_init( res );

    double startTime = (float)clock()/CLOCKS_PER_SEC;

    mpz_fib_ui( res, 4784969 );

    double endTime = (float)clock()/CLOCKS_PER_SEC;
    double timeElapsed = endTime - startTime;

    gmp_printf( "%Zd\n", res );

    printf("%f\n", timeElapsed);
}

