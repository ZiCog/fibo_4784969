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

// Fibo as above with OMP
const bint fiboOmp (int n, int level) {
    bint a;
    bint b;
    bint res;
    bool found = false;

    #pragma omp critical
    if (memo.find(n) != memo.end()) {
        res = memo[n];
        found = true;    
    }
    if (found) {
        return res;
    }
    
    int k = (n / 2);

    if (level < 4) {

        int nthreads, tid;
        #pragma omp parallel shared(k, a, b, memo, nthreads, level) private(n, tid)
        {

            tid = omp_get_thread_num();
            #pragma omp sections
            {
                #pragma omp section
                {
                    #pragma omp critical
                    std::cout << "Thread " << tid << " doing section 1" << std::endl;

                    a = fiboOmp(k, level + 1);
                }
                #pragma omp section
                {
                    #pragma omp critical
                    std::cout << "Thread " << tid << " doing section 2" << std::endl;

                    b = fiboOmp(k - 1, level + 1);
                }
            }  // End of sections
        }  //  End of parallel section
    } else {
        a = fiboOmp(k, level + 1);
        b = fiboOmp(k - 1, level + 1);
    }

    // NOTE: We removed memo here for a two times slow down!
    if (isEven(n)) {
        res = a * (two * b + a);
        #pragma omp critical
        memo[n] = res;
        return res;
    }
    if ((n % 4) == 1) {
        res = (two * a + b) * (two * a - b) + two;
        #pragma omp critical
        memo[n] = res;
        return res;
    }
    res = (two * a + b) * (two * a - b) - two;
    #pragma omp critical
    memo[n] = res;
    return res;
}

int main(int argc, char *argv[]) {
    int n = 4784969;
    bint res; 

    if (argc >= 2) {
        n = atol(argv[1]);
    }

    const char* command = "omp";
    bool useOmp = false;
    if ((argc == 3) && (strncmp(argv[2], command, strlen(command)) == 0)) {
        useOmp = true;
        std::cout << "Using OMP:" << std::endl;
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
	res = fiboOmp(n, 0);
    } else {
        res = fibo(n);
    }

    std::cout << res << std::endl;

    return 0;
}
