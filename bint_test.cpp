#include <bint.h>
#include <time.h> 

// Construct, NULL value
void test_01 (void)
{
    for (int i = 0; i < 10; i++)
    {
        bint x = bint();
        std::cout << "x: " << x << std::endl;
    }
}

// Construct from size_t
void test_02 (void)
{
    for (int i = 0; i < 10; i++)
    {
        bint x = bint(8);
        std::cout << "x: " << x << std::endl;
    }
}

// Construct from string
void test_03 (void)
{
    for (int i = 0; i < 10; i++)
    {
        bint x = "1234567899999999";
        std::cout << "x: " << x << std::endl;
    }
}

// Grow
void test_04 (void)
{
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
}

//
void test_06 (void)
{
}

// Sum, same size
void test_07 (void)
{
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
    bint x = "0";
    bint one = "1";

    for (int i = 0; i < 1000; i++)
    {
        x = x.sum(one);
        std::cout << "x: " << x << std::endl;
    }
}

bint fibo_recursive(int n)
{
    if (n < 2)
    {
        return bint(n);
    }
    else
    {
        return fibo_recursive(n - 2).sum(fibo_recursive(n - 1));
    }
}

// Recursive fibo
void test_14 (void)
{
    bint f = fibo_recursive(12);
    std::cout << "f: " << f << std::endl;
}

void fibo()
{
    bint f0 = "0";
    bint f1 = "1";
    bint f2;
    
    //int k = 4784969;
    int k = 12;

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
    fibo();
}

// low / high
void test_16 (void)
{
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
    bint x = "4444444444444444";
    bint y = "1111116111111115";
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    bint res = x.sub(y);
    std::cout << "res: " << res << std::endl;
}

// Sub, shorter from longer
void test_18 (void)
{
    bint x = "4444444444444444";
    bint y = "1116111111115";
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    bint res = x - y;
    std::cout << "res: " << res << std::endl;
}

// Sub, longer from shorter (expect assertion failure)
void test_19 (void)
{
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
    bint x = "1234";
    bint y = "5678";
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    bint res = x.mul(y);
    std::cout << "res: " << res << std::endl;
}

// mul, with big integers
void test_23 (void)
{
    bint x = "99999999";
    bint y = "99999999";
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    bint res = x.mul(y);
    std::cout << "res: " << res << std::endl;
}

// mul, with big integers
void test_24 (void)
{
    bint x = "12345678";
    bint y = "87654321";
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    bint res = x.mul(y);
    std::cout << "res: " << res << std::endl;
}

// mul, with big integers
void test_25 (void)
{
    bint x = "3463463462345678";
    bint y = "7654321634677457";

    double startTime = (float)clock()/CLOCKS_PER_SEC;

    bint res;
    res = x.mul(y);
    double endTime = (float)clock()/CLOCKS_PER_SEC;
    double timeElapsed = endTime - startTime;
    std::cout << "Elapsted time: " << timeElapsed << std::endl;

    std::cout << "res: " << res << std::endl;
}

extern int allocCount;

int main (int argc, char* argv[])
{
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
//    test_14();   Skip recursive fibo()
    test_15();
    test_16();
    test_17();
    test_18();
//    test_19();   FAILS: As it should!
    test_20();
    test_21();
    test_22();
    test_23();    // PASS !!
    test_24();    // PASS !!

    allocCount = 0;


    test_25();    // PASS !!


    std::cout << "Number of array allocations: " << allocCount << std::endl;

    return 0;
}

