#include <time.h>
#include <unordered_map>
#include <time.h>

#include "bint.h"

bint zero = "0";
bint one = "1";
bint two = "2";

int isEven(int n) { return (n & 1) == 0; }

#if MUTABLE
std::unordered_map<uint64_t, bint> memo;

bint fibo(int n) {
    if (memo.find(n) != memo.end()) {
        return memo[n];    
    }
    int k = (n / 2);
    bint fk = fibo(k);
    bint fk1 = fibo(k + 1);
    if (isEven(n)) {
        bint result = fk * (fk1 * two - fk); 
        memo[n] = result;
        return result;
    }
    bint result = (fk * fk) + (fk1 * fk1);
    memo[n] = result;
    return result; 
}
#else
bint fibo (int n)
{
    switch (n)
    {
        case 0:
            return bint("0");
        case 1:
            return bint("1");
        case 2:
            return bint("1");
        default:
            int k = (n / 2);
            bint fk = fibo(k);
            bint fk1 = fibo(k + 1);
            if (isEven(n))
            {
                bint x = fk1 * two - fk;
                bint res = fk * x;
                return res;
            }
            bint t1 = fk * fk;
            bint t2 = fk1 * fk1;
            bint res = t1 + t2;
            return res;
    }
}
#endif

// This is the way ejolson did it. It's a bit slower than my fibo above
static void fibo_ejolson(int n, bint& a, bint& b) {
    if( n == 0 ) {
        a = bint("0");
        b = bint("1");
        return;
    }
    fibo_ejolson(n / 2, a, b);
    bint taa = a * a;
    bint tbb = b * b;
    bint taapbb = taa + tbb;
    if(n & 1) {
        b = b * (a + a + b);
        a = taapbb;
    } else {
        a = a * (b + b - a);
        b = taapbb;
    }
}

bint timeIt(int n) {
    double endTime;
    double elapsedTime;
    double startTime = (float)clock()/CLOCKS_PER_SEC;

#if MUTABLE
    memo.clear();
    memo[0] = zero;
    memo[1] = one;
    memo[2] = one;
#endif
    bint res = fibo(n);

    endTime = (float)clock()/CLOCKS_PER_SEC;
    elapsedTime = endTime - startTime;
    std::cout << "Compute time: " << elapsedTime << std::endl;

    return res;
}

int main(int argc, char *argv[]) {
#if 1
    bint res = timeIt(4784969);
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
    std::cout << "allocGrow: " << allocGrow << std::endl;
    std::cout << "allocBytes: " << allocBytes << std::endl;
#endif

#else
    bint a;
    bint b;
    fibo_ejolson(4784969, a, b);
    std::cout << a << std::endl;
#endif
    return 0;
}
