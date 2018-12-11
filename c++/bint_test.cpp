#include <bint.h>
#include <time.h> 

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

    for (int i = 0; i < 10; i++)
    {
        bint x = bint(8);
        std::cout << "x: " << x << std::endl;
    }
}

// Construct from string
void test_03 (void)
{
    std::cout << std::endl << "Test 03: " << std::endl;

    for (int i = 0; i < 10; i++)
    {
        bint x = "1234567899999999";
        std::cout << "x: " << x << std::endl;
    }
}

// Grow
void test_04 (void)
{
    std::cout << std::endl << "Test 04: " << std::endl;

    {
        bint x = "12345678";
        std::cout << "x: " << x << std::endl;
        std::cout << "Grow:" << std::endl;
        x.grow();
        std::cout << "x: " << x << std::endl;
    }
    {
        bint x;
        x = "12345678";
        std::cout << "x: " << x << std::endl;
        std::cout << "Grow:" << std::endl;
        x.grow();
        std::cout << "x: " << x << std::endl;
    }
    {
        bint x("12345678");
        std::cout << "x: " << x << std::endl;
        std::cout << "Grow:" << std::endl;
        x.grow();
        std::cout << "x: " << x << std::endl;
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
    bint res = x.sum(y);
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
    bint res = x.sum(y);
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
    bint res = x.sum(y);
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
    bint res = x.sum(y);
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

    bint z = x.sum(one).sum(two).sum(three).sum(four).sum(five).sum(six);
    std::cout << "z: " << z << std::endl;

    z = one + two + three + four + five + six;
    std::cout << "z: " << z << std::endl;
}

// A counter
void test_13 (void)
{
    std::cout << std::endl << "Test 13: " << std::endl;

    bint x = "0";
    bint one = "1";

    for (int i = 0; i < 1000; i++)
    {
        x = x.sum(one);
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
            return fibo_recursive(n - 2).sum(fibo_recursive(n - 1));
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
    int k = 14;

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

    bint x = "1234567887654321";
    std::cout << "x: " << x << std::endl;
    bint low = x.low();
    std::cout << "low: " << low << std::endl;

    bint high = x.high();
    std::cout << "high " << high << std::endl;
}

// Sub, same size
void test_17 (void)
{
    std::cout << std::endl << "Test 17: " << std::endl;

    bint x = "4444444444444444";
    bint y = "1111116111111115";
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    bint res = x.sub(y);
    std::cout << "res: " << res << std::endl;

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
    bint res = x.sub(y);
    std::cout << "res: " << res << std::endl;
}

// mul, the base case, one element in value, no overflow
void test_20 (void)
{
    std::cout << std::endl << "Test 20: " << std::endl;

    bint x = "2";
    bint y = "3";
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    bint res = x.mul(y);
    std::cout << "res: " << res << std::endl;
}

// mul, the base case, one element in value, with overflow
void test_21 (void)
{
    std::cout << std::endl << "Test 21: " << std::endl;

    bint x = "9";
    bint y = "9";
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    bint res = x.mul(y);
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
    bint res = x.mul(y);
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
    bint res = x.mul(y);
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
    bint res = x.mul(y);
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

    bint res;
    res = x.mul(y);
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
        res = res.mul(two);
        res.shrink(32);
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
        res1 = res1.mul(two);
        res1.shrink(32);
    }
    std::cout << "res1: " << res1 << std::endl;

    bint res2 = res1 * res1;
    std::cout << "res2: " << res2 << std::endl;
}

extern int allocCount;

int main (int argc, char* argv[])
{
    std::cout << "DIGITS = " << DIGITS << std::endl;
    std::cout << "BASE = " << BASE << std::endl;
    std::cout << "LIMIT = " << LIMIT << std::endl;
    exit(1);

    test_01();   // PASS !!
    test_02();   // PASS !!
    test_03();   // PASS !!
    test_04();   // PASS !!
    test_05();   // PASS !!
    test_06();   // PASS !!
    test_07();   // PASS !!
    test_08();   // PASS !!
    test_09();   // PASS !!
    test_10();   // PASS !!
    test_11();   // PASS !!
    test_12();   // PASS !!
    test_13();   // PASS !!
    test_14();   // PASS !!
    test_15();   // PASS !!
    test_16();   // PASS !!
    test_17();   // PASS !!
    test_18();   // PASS !!
//    test_19();   // FAILS: As it should!
    test_20();   // PASS !!
    test_21();   // PASS !!
    test_22();   // PASS !!
    test_23();   // PASS !!
    test_24();   // PASS !!
    test_25();   // PASS !!
    test_26();   // PASS !!

    allocCount = 0;

    test_27();   // FAIL  subtracts going negative

    std::cout << "Number of array allocations: " << allocCount << std::endl;

    return 0;
}

