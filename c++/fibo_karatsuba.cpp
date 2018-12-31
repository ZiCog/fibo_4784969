#include <time.h>
#include <unordered_map>
#include <time.h>
#include <omp.h>
#include <string.h>
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

int threads = 0;

// Fibo as above with OMP
const bint fiboOmp (int n) {
    bint a;
    bint b;
    bint res;

    if (n == 0) {
        res = zero;
        return res;
    }
    if (n == 1) {
        res = one;
        return res;
    }

    int k = (n / 2);

    int t;
    #pragma omp atomic read
    t = threads;
    if (t <= 16) {

        int tid;
        #pragma omp atomic
        threads++;
        #pragma omp parallel shared(k, a, b, memo, threads) private(tid, n)
        {
            tid = omp_get_thread_num();
            #pragma omp sections
            {
                #pragma omp section
                {
                    #pragma omp critical
                    std::cout << "Thread " << tid << " doing section 1" << '\n';

                    a = fiboOmp(k);
                }
                #pragma omp section
                {
                    #pragma omp critical
                    std::cout << "Thread " << tid << " doing section 2" << '\n';

                    b = fiboOmp(k - 1);
                }
            }  // End of sections
        }  //  End of parallel section
        #pragma omp atomic
        threads--;
    } else {
        a = fiboOmp(k);
        b = fiboOmp(k - 1);
    }

    if (isEven(n)) {
        res = a * (two * b + a);
        return res;
    }
    if ((n % 4) == 1) {
        res = (two * a + b) * (two * a - b) + two;
        return res;
    }
    res = (two * a + b) * (two * a - b) - two;
    return res;
}

int main(int argc, char *argv[]) {
    int n = 4784969;   // The first Fibonacci number with a million digits
    bint res; 

    if (argc >= 2) {
        n = atol(argv[1]);
    }

    const char* command = "omp";
    bool useOmp = false;
    if ((argc == 3) && (strncmp(argv[2], command, strlen(command)) == 0)) {
        useOmp = true;
    }

    // Initialize the fibo's memo.
    memo.clear();
    memo[0] = zero;
    memo[1] = one;
    memo[2] = one;

    if (useOmp) {
        int procs = omp_get_num_procs();    
        std::cout << "Using OMP on " << procs << " cores." << std::endl;
        omp_set_nested(true);
	res = fiboOmp(n);
    } else {
        res = fibo(n);
    }

    std::cout << res << std::endl;

    return 0;
}
