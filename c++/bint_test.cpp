#include <gmpxx.h>
#include <time.h> 
#include "bint.h"

// Construct, NULL value
void test_01 (void)
{
    std::cout << std::endl << "Test 01: " << std::endl;

    for (int i = 0; i < 10; i++)
    {
        bint x = bint();
        std::cout << "x: " << x << std::endl;
    }
}

// Construct from size_t
void test_02 (void)
{
    std::cout << std::endl << "Test 02: " << std::endl;

    for (int i = 0; i < 8; i++)
    {
        bint x = bint(i + 1);
        std::cout << "x: " << x << std::endl;
    }
}

// Construct from string
void test_03 (void)
{
    std::cout << std::endl << "Test 03: " << std::endl;

    bint x = "1111111222222233333334444444";
    std::cout << "x: " << x << std::endl;

    bint y = "11111112222222333333344444445";
    std::cout << "y: " << y << std::endl;

    bint z = "11111112222222333333344444445555";
    std::cout << "z: " << z << std::endl;

    bint q = "1111111222222233333334444444555555";
    std::cout << "q: " << q << std::endl;

    bint t = "";
    std::cout << "t: " << t << std::endl;
}

// Sum, zero + zero
void test_04 (void)
{
    {
        std::cout << std::endl << "Test 04_a: " << std::endl;

        bint x = "0";
        bint y = "0";
        std::cout << "x: " << x << std::endl;
        std::cout << "y: " << y << std::endl;
        bint res = x + y;
        std::cout << "res: " << res << std::endl;
    }
    {
        std::cout << std::endl << "Test 04_b: " << std::endl;

        bint x = "999999999999999999";
        bint y = "0";
        std::cout << "x: " << x << std::endl;
        std::cout << "y: " << y << std::endl;
        bint res = x + y;
        std::cout << "res: " << res << std::endl;
    }
    {
        std::cout << std::endl << "Test 04_c: " << std::endl;

        bint x = "0";
        bint y = "999999999999999999";
        std::cout << "x: " << x << std::endl;
        std::cout << "y: " << y << std::endl;
        bint res = x + y;
        std::cout << "res: " << res << std::endl;
    }
    {
        std::cout << std::endl << "Test 04_d: " << std::endl;

        bint x;
        bint y = "999999999999999999";
        std::cout << "x: " << x << std::endl;
        std::cout << "y: " << y << std::endl;
        bint res = x + y;
        std::cout << "res: " << res << std::endl;
    }
    {
        std::cout << std::endl << "Test 04_e: " << std::endl;

        bint x = "999999999999999999";
        bint y;
        std::cout << "x: " << x << std::endl;
        std::cout << "y: " << y << std::endl;
        bint res = x + y;
        std::cout << "res: " << res << std::endl;
    }
}

// 
void test_05 (void)
{
    std::cout << std::endl << "Test 05: " << std::endl;
}

//
void test_06 (void)
{
    std::cout << std::endl << "Test 06: " << std::endl;
}

// Sum, same size
void test_07 (void)
{
    std::cout << std::endl << "Test 07: " << std::endl;

    bint x = "1111111111111111";
    bint y = "4444444444444444";
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    bint res = x + y;
    std::cout << "res: " << res << std::endl;
}

// Sum, small big
void test_08 (void)
{
    std::cout << std::endl << "Test 08: " << std::endl;

    bint x = "11111111";
    bint y = "4444444444444444";
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    bint res = x + y;
    std::cout << "res: " << res << std::endl;
}

// Sum, big + small
void test_09 (void)
{
    std::cout << std::endl << "Test 09: " << std::endl;

    bint x = "1111111111111111";
    bint y = "44444444";
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    bint res = x + y;
    std::cout << "res: " << res << std::endl;
}

// Sum, small + big with carry
void test_10 (void)
{
    std::cout << std::endl << "Test 10: " << std::endl;

    bint x = "11111111";
    bint y = "9999999999999999";
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    bint res = x + y;
    std::cout << "res: " << res << std::endl;
}

// Sum, big + small with carry
void test_11 (void)
{
    std::cout << std::endl << "Test 11: " << std::endl;

    bint x = "9999999999999999";
    bint y = "11111111";
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    bint res = x + y;
    std::cout << "res: " << res << std::endl;
}

// Chain operations
void test_12 (void)
{
    std::cout << std::endl << "Test 12: " << std::endl;

    bint x = "0";
    bint one = "1000";
    bint two = "2000";
    bint three = "3000";
    bint four = "4000";
    bint five = "5000";
    bint six = "6000";
    std::cout << "x: " << x << std::endl;

    bint z = x + one + two + three + four + five + six;
    std::cout << "z: " << z << std::endl;
}

// A counter
void test_13 (void)
{
    std::cout << std::endl << "Test 13: " << std::endl;

    bint x = "0";
    bint one = "1";

    for (int i = 0; i < 1000000; i++)
    {
        x = x + one;
    }
    std::cout << "x: " << x << std::endl;
}

bint fibo_recursive(int n)
{
    switch (n)
    {
        case 0:
            return bint("0");
            break;
        case 1:
            return bint("1");
            break;
        case 2:
            return bint("1");
            break;
        default:
            return fibo_recursive(n - 2) + fibo_recursive(n - 1);
    }
}

// Recursive fibo
void test_14 (void)
{
    std::cout << std::endl << "Test 14: " << std::endl;

    bint f = fibo_recursive(14);
    std::cout << "f: " << f << std::endl;
}

void fibo()
{
    bint f0 = "0";
    bint f1 = "1";
    bint f2;
    
    //int k = 4784969;
    int k = 20000;

    int n = 2;
    while (1) 
    {
        f2 = f0 + f1;
        if (n == k)
        {
            std::cout << "fibo " << n << " = ";
            std::cout << f2 << std::endl;
            break;
        }
        n++;
        f0 = f1 + f2;
        if (n == k)
        {
            std::cout << "fibo " << n << " = ";
            std::cout << f0 << std::endl;
            break;
        }
        n++;
        f1 = f2 + f0;
        if (n == k)
        {
            std::cout << "fibo " << n << " = ";
            std::cout << f1 << std::endl;
            break;
        }
        n++;
    }
}

// Schoolboy fibo
void test_15 (void)
{
    std::cout << std::endl << "Test 15: " << std::endl;

    fibo();
}

// low / high
void test_16 (void)
{
    std::cout << std::endl << "Test 16: " << std::endl;

    bint x = "999999999999999999"
             "888888888888888888"
             "777777777777777777"
             "666666666666666666"
             "555555555555555555";
    std::cout << "x: " << x << std::endl;
    bint low = x.low(2);
    std::cout << "low: " << low << std::endl;

    bint high = x.high(2);
    std::cout << "high " << high << std::endl;
}

// Sub, same size
void test_17 (void)
{
    {
        std::cout << std::endl << "Test 17: " << std::endl;

        bint x = "0";
        bint y = "0";
        std::cout << "x: " << x << std::endl;
        std::cout << "y: " << y << std::endl;
        bint res = x - y;
        std::cout << "res: " << res << std::endl;
    }
    {
        std::cout << std::endl << "Test 17_b: " << std::endl;

        bint x = "999999999999999999";
        bint y = "0";
        std::cout << "x: " << x << std::endl;
        std::cout << "y: " << y << std::endl;
        bint res = x - y;
        std::cout << "res: " << res << std::endl;
    }
    {
        std::cout << std::endl << "Test 17_c: " << std::endl;

        bint x = "999999999999999999""999999999999999999""000000000000000000""000000000000000000";
        bint y = "999999999999999999""999999999999999999";
        std::cout << "x: " << x << std::endl;
        std::cout << "y: " << y << std::endl;
        bint res = x - y;
        std::cout << "res: " << res << std::endl;
    }
    {
        std::cout << std::endl << "Test 17_d: " << std::endl;

        bint x = "999999999999999999""000000000000000000""000000000000000000""000000000000000000";
        bint y = "999999999999999999""999999999999999999";
        std::cout << "x: " << x << std::endl;
        std::cout << "y: " << y << std::endl;
        bint res = x - y;
        std::cout << "res: " << res << std::endl;
    }
    {
        std::cout << std::endl << "Test 17_e: " << std::endl;

        bint x = "4444444444444444";
        bint y = "1111116111111115";
        std::cout << "x: " << x << std::endl;
        std::cout << "y: " << y << std::endl;
        bint res = x - y;
        std::cout << "res: " << res << std::endl;
    }
    // expect "3333328333333329"
}

// Sub, shorter from longer
void test_18 (void)
{
    std::cout << std::endl << "Test 18: " << std::endl;

    bint x = "4444444444444444";
    bint y = "1116111111115";
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    bint res = x - y;
    std::cout << "res: " << res << std::endl;

    // expect "4443328333333329"
}

// Sub, longer from shorter (expect assertion failure)
void test_19 (void)
{
    std::cout << std::endl << "Test 19: " << std::endl;

    bint x = "44444";
    bint y = "1111116111111115";
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    bint res = x - y;
    std::cout << "res: " << res << std::endl;
}

// simpleMul
void test_20_1 (void)
{
    std::cout << std::endl << "Test 20_1: " << std::endl;

    bint a = "2";
    std::cout << "a: " << a << std::endl;
    bint res1 = a.simpleMul(3);
    std::cout << "res1: " << res1 << std::endl;

    bint b = "2222222222222222222222222222";
    std::cout << "b: " << a << std::endl;
    bint res2 = b.simpleMul(3);
    std::cout << "res2: " << res2 << std::endl;

    bint c = "9999999999999999999999999999";
    std::cout << "b: " << c << std::endl;
    bint res3 = c.simpleMul(9);
    std::cout << "res3: " << res3 << std::endl;
}

// mul, the base case, one element in value, no overflow
void test_20 (void)
{
    std::cout << std::endl << "Test 20: " << std::endl;
    {
        bint x = "2";
        bint y = "3";
        std::cout << "x: " << x << std::endl;
        std::cout << "y: " << y << std::endl;
        bint res = x * y;
        std::cout << "res: " << res << std::endl;
    }
    {
        bint x = "999999999999999999";
        bint y = "2";
        std::cout << "x: " << x << std::endl;
        std::cout << "y: " << y << std::endl;
        bint res = x * y;
        std::cout << "res: " << res << std::endl;
    }
    {
        bint x = "2";
        bint y = "999999999999999999";
        std::cout << "x: " << x << std::endl;
        std::cout << "y: " << y << std::endl;
        bint res = x * y;
        std::cout << "res: " << res << std::endl;
    }
    {
        bint x = "999999999999999999";
        bint y = "999999999999999999";
        std::cout << "x: " << x << std::endl;
        std::cout << "y: " << y << std::endl;
        bint res = x * y;
        std::cout << "res: " << res << std::endl;
    }
}

// mul, the base case, one element in value, with overflow
void test_21 (void)
{
    std::cout << std::endl << "Test 21: " << std::endl;

    bint x = "9";
    bint y = "9";
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    bint res = x * y;
    std::cout << "res: " << res << std::endl;
}

// mul, testing spilt, four elements into 1
void test_22 (void)
{
    std::cout << std::endl << "Test 22: " << std::endl;

    bint x = "1234";
    bint y = "5678";
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    bint res = x * y;
    std::cout << "res: " << res << std::endl;

    // Expect: "7006652"
}

// mul, with big integers
void test_23 (void)
{
    std::cout << std::endl << "Test 23: " << std::endl;

    bint x = "99999999";
    bint y = "99999999";
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    bint res = x * y;
    std::cout << "res: " << res << std::endl;

    // Excpect "9999999800000001"
}

// mul, with big integers
void test_24 (void)
{
    std::cout << std::endl << "Test 24: " << std::endl;

    bint x = "12345678";
    bint y = "87654321";
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    bint res = x * y;
    std::cout << "res: " << res << std::endl;

    // Expect "1082152022374638"
}

// mul, with big integers
void test_25 (void)
{
    std::cout << std::endl << "Test 25: " << std::endl;

    bint x = "3463463462345678";
    bint y = "7654321634677457";

    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;


    double startTime = (float)clock()/CLOCKS_PER_SEC;

    bint res = x * y;
    double endTime = (float)clock()/CLOCKS_PER_SEC;
    double timeElapsed = endTime - startTime;
    std::cout << "Elapsted time: " << timeElapsed << std::endl;

    std::cout << "res: " << res << std::endl;

    // Expect "26510463310747415068608167980846"
}

// mul, with big integers
void test_26 (void)
{
    std::cout << std::endl << "Test 26: " << std::endl;

    bint two = "2";
    bint res = "1";

    std::cout << "two: " << two << std::endl;

    for (int i = 0; i < 1000; i++)
    {   
        res = res * two;
    }
    std::cout << "res: " << res << std::endl;
}

// mul, with big integers
void test_27 (void)
{
    std::cout << std::endl << "Test 27: " << std::endl;

    bint two = "2";
    bint res1 = "1";

    std::cout << "two: " << two << std::endl;

    for (int i = 0; i < 1000; i++)
    {
        res1 = res1 * two;
    }
    std::cout << "res1: " << res1 << std::endl;

    bint res2 = res1 * res1;
    std::cout << "res2: " << res2 << std::endl;
}

/*
bint zero = "0";
bint one = "1";
bint two = "2";
*/
int isEven(int n)
{
    return (n & 1) == 0;
}

bint fibok (int n)
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
            bint fk = fibok(k);
            bint fk1 = fibok(k + 1);
            if (isEven(n))
            {
                bint x = fk1 * bint("2") - fk;
                bint res = fk * x;
                return res;
            }
            bint t1 = fk * fk;
            bint t2 = fk1 * fk1;
            bint res = t1 + t2;
            return res;
    }
}

// mul, via calculating all fibo up to 20000
void test_28 (void)
{
    std::cout << "Test 28: ";

    for (int i = 0; i <= 20000; i++) {
        bint res = fibok  (i);

//        std::cout << i << ", ";
//        std::cout << res;
//        std::cout << std::endl;

        mpz_t f;
        mpz_init(f);
        mpz_fib_ui(f, i);
        mpz_class expected(f);
        mpz_clear(f);
        bint x = bint(expected.get_str().c_str());

        if (x != res) {
            std::cout << "FAIL." << std::endl;
            return;
        }
    }
    std::cout << "PASS." << std::endl;
}

void timeIt(int n) {

    double startTime;
    double endTime;
    double elapsedTime;

    startTime = (float)clock()/CLOCKS_PER_SEC;

    bint res = fibok(n);

    endTime = (float)clock()/CLOCKS_PER_SEC;
    elapsedTime = endTime - startTime;

    std::cout << n << " , " << elapsedTime << std::endl;
}

void test_29 (void)
{
    std::cout << std::endl << "Test 29: " << std::endl;

    for (int n = 2; n <= 1024 * 1024 * 32; n *= 2) {
        timeIt(n);
    }
}

// This is the way ejolson did it. It's a bit slower than fibok above
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
        // [a, b] = [a*a + b*b, b*(2*a + b)]
        b = b * (a + a + b);
        a = taapbb;
    } else {
        // [a, b] = [a*(b*2 - a), a*a + b*b]
        a = a * (b + b -  a);
        b = taapbb;
    }
}

void test_30 (void)
{
    std::cout << std::endl << "Test 30: ";

    bint res;
    bint b;
    int n = 4784969;

    fibo_ejolson(n, res, b);

//    std::cout << res << std::endl;

    mpz_t f;
    mpz_init(f);
    mpz_fib_ui(f, n);
    mpz_class expected(f);
    mpz_clear(f);
    bint x = bint(expected.get_str().c_str());

    if (x != res) {
        std::cout << "FAIL." << std::endl;
        return;
    }
    std::cout << "PASS." << std::endl;
}

bint factorial(bint n)
{
    if (n == bint("0"))
        return bint("1");
    else
        return(n * factorial(n - bint("1")));
}

void test_31 (void)
{
    std::cout << std::endl << "Test 31: " << std::endl;

    bint res = factorial(bint("100"));
    std::cout << "Expect: " << "93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000" << std::endl;
    std::cout << "        " << res << std::endl;
}

void test_32 (void)
{
    std::cout << std::endl << "Test 32: " << std::endl;

    bint a = "440047129495446";
    bint b = "26819189637968393739987639";
    bint c = "26819189637968393739987639";
    int aShift = 2;
    int bShift = 1;
    //result.width = 4
    //a.width =      1
    //b.width =      2
    bint res0 = a.shiftAndAdd(a, b, c, aShift, bShift);
    std::cout << "shiftAndAdd: " << res0 << std::endl;

/*

    bint a = "111111111111111111""111111111111111111""111111111111111111""111111111111111111""111111111111111111"
             "111111111111111111""111111111111111111""111111111111111111""111111111111111111""111111111111111111"
             "111111111111111111""111111111111111111""111111111111111111""111111111111111111""111111111111111111"
             "111111111111111111""111111111111111111""111111111111111111""111111111111111111""111111111111111111"
             "111111111111111111""111111111111111111""111111111111111111""111111111111111111""111111111111111111"
             "111111111111111111""111111111111111111""111111111111111111""111111111111111111""111111111111111111"
             "111111111111111111""111111111111111111""111111111111111111""111111111111111111""111111111111111111"
             "111111111111111111""111111111111111111""111111111111111111""111111111111111111""111111111111111111"
             "111111111111111111""111111111111111111""111111111111111111""111111111111111111""111111111111111111"
             "111111111111111111""111111111111111111""111111111111111111""111111111111111111""111111111111111111"
             "111111111111111111""111111111111111111""111111111111111111""111111111111111111""111111111111111111"
             "111111111111111111""111111111111111111""111111111111111111""111111111111111111";
    bint b = "222222222222222222""222222222222222222""222222222222222222""222222222222222222""222222222222222222"
             "222222222222222222""222222222222222222""222222222222222222""222222222222222222""222222222222222222"
             "222222222222222222""222222222222222222""222222222222222222""222222222222222222""222222222222222222"
             "222222222222222222""222222222222222222""222222222222222222""222222222222222222""222222222222222222"
             "222222222222222222""222222222222222222""222222222222222222""222222222222222222""222222222222222222"
             "222222222222222222""222222222222222222""222222222222222222""222222222222222222""222222222222222222"
             "222222222222222222""222222222222222222""222222222222222222""222222222222222222""222222222222222222"
             "222222222222222222""222222222222222222""222222222222222222""222222222222222222""222222222222222222"
             "222222222222222222""222222222222222222""222222222222222222""222222222222222222""222222222222222222"
             "222222222222222222""222222222222222222""222222222222222222""222222222222222222""222222222222222222"
             "222222222222222222""222222222222222222""222222222222222222""222222222222222222""222222222222222222"
             "222222222222222222""222222222222222222""222222222222222222""222222222222222222";
    bint c = "333333333333333333""333333333333333333""333333333333333333""333333333333333333""333333333333333333"
             "333333333333333333""333333333333333333""333333333333333333""333333333333333333""333333333333333333"
             "333333333333333333""333333333333333333""333333333333333333""333333333333333333""333333333333333333"
             "333333333333333333""333333333333333333""333333333333333333""333333333333333333""333333333333333333"
             "333333333333333333""333333333333333333""333333333333333333""333333333333333333""333333333333333333"
             "333333333333333333""333333333333333333""333333333333333333""333333333333333333""333333333333333333"
             "333333333333333333""333333333333333333""333333333333333333""333333333333333333""333333333333333333"
             "333333333333333333""333333333333333333""333333333333333333""333333333333333333""333333333333333333"
             "333333333333333333""333333333333333333""333333333333333333""333333333333333333""333333333333333333"   
             "333333333333333333""333333333333333333""333333333333333333""333333333333333333""333333333333333333"
             "333333333333333333""333333333333333333""333333333333333333""333333333333333333""333333333333333333"
             "333333333333333333""333333333333333333""333333333333333333";

    bint res0 = a.shiftAndAdd(a, b, c, 36, 0);              // 35 works here!
    std::cout << "shiftAndAdd: " << res0 << std::endl;
*/
}

mpz_class pow(mpz_class x, int n)  {
    if (n == 0) {
        return mpz_class("1");
    }
    mpz_class result = x;
    for (int i = 0; i < n; i++) {
        result = result * mpz_class(10);
    }
    return result;
}


// naiveMull()
void test_34 (void)
{
    std::cout << std::endl << "Test 34: " << std::endl;
    {
        bint x = "2";
        bint y = "3";

        bint res = x.naiveMul(y);

        std::cout << "test_34, 0: " << res << std::endl;
    }
    {
        bint x = "222222222222222222";
        bint y = "3";

        bint res = x.naiveMul(y);

        std::cout << "test_34, 1: " << res << std::endl;
    }
    {
        bint x = "2";
        bint y = "333333333333333333";

        bint res = x.naiveMul(y);

        std::cout << "test_34, 2: " << res << std::endl;
    }
    {
        bint x = "222222222222222222222222222222222222";
        bint y = "3";

        bint res = x.naiveMul(y);

        std::cout << "test_34, 3: " << res << std::endl;
    }
    {
        bint x = "2";
        bint y = "333333333333333333333333333333333333";

        bint res = x.naiveMul(y);

        std::cout << "test_34, 4: " << res << std::endl;
    }
    {
        bint x = "999999999999999999999999999999999999";
        bint y = "9";

        bint res = x.naiveMul(y);

        std::cout << "test_34, 5: " << res << std::endl;
    }
    {
        bint x = "9";
        bint y = "999999999999999999999999999999999999";

        bint res = x.naiveMul(y);

        std::cout << "test_34, 6: " << res << std::endl;
    }
    {
        bint x = "111111111111111111222222222222222222";
        bint y = "333333333333333333444444444444444444";

        bint res = x.naiveMul(y);

        std::cout << "test_34, 7: " << std::endl;
        std::cout << "Expect:" << "37037037037037037086419753086419752975308641975308641901234567901234568" << std::endl;
        std::cout << "       " << res << std::endl;

    }
    {
        std::cout << "test_34, 8: " << std::endl;
        std::cout << "Expect:" << "1371742112482853227251943301326017369608291418991007451912818167962200896357262612406645354671543971955494572778539856729157126047858558146624" << std::endl;
        bint x = "37037037037037037086419753086419752975308641975308641901234567901234568";

        bint res = x.naiveMul(x);

        std::cout << "       " << res << std::endl;
    }
}

// operator*() random fuzzing
void test_35 (void)
{
    std::cout << std::endl << "Test 35: ";

    gmp_randclass  r(gmp_randinit_default);

    mpz_class randomRange = 1;
    for (int i = 1; i <= 1000; i++) {
        randomRange *= 10;
//        std::cout << "Random range: " << randomRange << std::endl;

        for (int j = 0; j < 10; j++) {
            mpz_class rand1 = r.get_z_range(randomRange); 
            mpz_class rand2 = r.get_z_range(randomRange); 

            std::string s1 =  rand1.get_str ();
            std::string s2 =  rand2.get_str ();

            bint b1 = s1.c_str();
            bint b2 = s2.c_str();

            mpz_class expected = rand1 * rand2;
            bint res = b1 * b2;

//            std::cout << "b1:           " << b1 << std::endl; 
//            std::cout << "b2:           " << b2 << std::endl; 
//            std::cout << "Expect:       " << expected << std::endl; 
//            std::cout << "Got:          " << res << std::endl; 

            bint x = bint(expected.get_str().c_str());

            if (x != res) {
                std::cout << "FAIL." << std::endl;
                return;
            }
        }
    }
    std::cout << "PASS." << std::endl;
}

// operator*() extreme limit
void test_36 (void)
{
    std::cout << "Test 36: ";

    for (int i = 0; i < 64; i++) {
        mpz_class p = pow(mpz_class(10), i * 17) - 1;
        bint b = p.get_str().c_str();

        mpz_class expected = p * p;
        bint res = b * b;

//        std::cout << "i: " << i << ", b: " << b << std::endl; 
//        std::cout << "Expect b * b : " << expected << std::endl; 
//        std::cout << "Got:           " << res << std::endl;

        bint x = bint(expected.get_str().c_str());

        if (x != res) {
            std::cout << "FAIL." << std::endl;
            return;
        }
    }
    std::cout << "PASS." << std::endl;
}

// naiveMul() random fuzzing
void test_37 (void)
{
    std::cout << "Test 37: ";

    gmp_randclass  r(gmp_randinit_default);

    mpz_class randomRange = 1;
    for (int i = 1; i <= 1000; i++) {
        randomRange *= 10;
//        std::cout << "Random range: " << randomRange << std::endl;

        for (int j = 0; j < 10; j++) {
            mpz_class rand1 = r.get_z_range(randomRange); 
            mpz_class rand2 = r.get_z_range(randomRange); 

            std::string s1 =  rand1.get_str ();
            std::string s2 =  rand2.get_str ();

            bint b1 = s1.c_str();
            bint b2 = s2.c_str();

//            std::cout << "b1:           " << b1 << std::endl; 
//            std::cout << "b2:           " << b2 << std::endl; 

            mpz_class expected = rand1 * rand2;
//            std::cout << "Expect:       " << expected  << std::endl; 

            bint res = b1.naiveMul(b2);
//            std::cout << "Got:          " << res << std::endl; 

            bint x = bint(expected.get_str().c_str());

            if (x != res) {
                std::cout << "FAIL." << std::endl;
                return;
            }
        }
    }
    std::cout << "PASS." << std::endl;
}

int main (void)
{
    std::cout << "DIGITS = " << DIGITS << std::endl;
    std::cout << "BASE = " << BASE << std::endl;

    test_01();   
    test_02();   
    test_03();   
    test_04();   
    test_05();   
    test_06();   
    test_07();   
    test_08();   
    test_09();   
    test_10();   
    test_11();   
    test_12();   
    test_13();
    test_14();   
    test_15();
    test_16();   
    test_17();   
    test_18();   
//    test_19();   // FAILS: As it should!
    test_20_1(); 
    test_20();   
    test_21();   
    test_22();
    test_23();
    test_24();
    test_25();
    test_26();
    test_27();
    test_28();
//    test_29();  // Too long to run
    test_30();
    test_31();
    test_32();
    test_34();
    test_35();
    test_36();
    test_37();

    return 0;
}

