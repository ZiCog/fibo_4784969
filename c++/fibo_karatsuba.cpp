#include <string.h>

#include "bint.h"
#include "fibo.h"
#include <omp.h>

int main(int argc, char *argv[]) {
    int n = 4784969;   // The first Fibonacci number with a million digits
    bint res; 

    if (argc >= 2) {
        n = atol(argv[1]);
    }

    fiboInit();
#ifdef USE_OMP
    #pragma omp parallel
    {
        #pragma omp single
        {
            fiboNew(n, res);
        }
    }
#else
    fiboNew(n, res);
#endif
    //res = fibo(n);

    std::cout << res << std::endl;

    return 0;
}
