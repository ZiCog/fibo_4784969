#include <bint.h>

// Construct, no params
void test_01 (void)
{
    for (int i = 0; i < 1000; i++)
    {
        bint x = bint();
        x.print();
    }
}

// Construct from integer
void test_02 (void)
{
    for (int i = 0; i < 1000; i++)
    {
        bint x = bint(4);
        x.print();
    }
}

// Construct from string
void test_03 (void)
{
    for (int i = 0; i < 1000; i++)
    {
        bint x = bint((char*)"1234567899999999");
        x.print();
    }
}

// Grow
void test_04 (void)
{
    bint x = bint((char*)"1234567812345678");
    x.print();
    for (int i = 0; i < 2; i++)
    {
        std::cout << "Grow:" << std::endl;
        x.grow();
        x.print();
    }
}

// Resise
void test_05 (void)
{
    bint x = bint((char*)"1234567812345678");
    x.print();
    int newSize = 32;
    for (int i = 0; i < 4; i++)
    {
        std::cout << "Resise:" << std::endl;
        x.resize(newSize *= 2);
        x.print();
    }
}

// Swap
void test_06 (void)
{
    bint x = bint((char*)"1111111111111111");
    bint y = bint((char*)"4444444444444444");
    x.print();
    y.print();
    x.swap(y);
    x.print();
    y.print();
    x.swap(y);
    x.print();
    y.print();
}

// Sum, same size
void test_07 (void)
{
    bint x = bint((char*)"1111111111111111");
    bint y = bint((char*)"4444444444444444");
    x.print();
    y.print();
    bint res = x.sum(y);
    res.print();
}

// Sum, small big
void test_08 (void)
{
    bint x = bint((char*)"11111111");
    bint y = bint((char*)"4444444444444444");
    x.print();
    y.print();
    bint res = x.sum(y);
    res.print();
}

// Sum, big + small
void test_09 (void)
{
    bint x = bint((char*)"1111111111111111");
    bint y = bint((char*)"44444444");
    x.print();
    y.print();
    bint res = x.sum(y);
    res.print();
}

// Sum, small + big with carry
void test_10 (void)
{
    bint x = bint((char*)"11111111");
    bint y = bint((char*)"9999999999999999");
    x.print();
    y.print();
    bint res = x + y;
    res.print();
}

// Sum, big + small with carry
void test_11 (void)
{
    bint x = bint((char*)"9999999999999999");
    bint y = bint((char*)"11111111");
    x.print();
    y.print();
    bint res = x.sum(y);
    res.print();
}

// Chain operations
void test_12 (void)
{
    bint x = bint((char*)"0");
    bint one = bint((char*)"1000");
    bint two = bint((char*)"2000");
    bint three = bint((char*)"3000");
    bint four = bint((char*)"4000");
    bint five = bint((char*)"5000");
    bint six = bint((char*)"6000");
    x.print();

    bint z = x.sum(one).sum(two).sum(three).sum(four).sum(five).sum(six);
    z.print();
    z = z + one + two + three + four + five + six;
    z.print();
}

// A counter
void test_13 (void)
{
    bint x = bint((char*)"0");
    bint one = bint((char*)"1");

    for (int i = 0; i < 10000; i++)
    {
        x = x.sum(one);
        x.print();
    }
}

bint f[] = {bint((char*)"0"), bint((char*)"1"), bint((char*)"1")};

bint fibo_recursive(int n)
{
    if (n <= 2)
    {
        return f[n];
    }
    else
    {
        return fibo_recursive(n - 2).sum(fibo_recursive(n - 1));
    }
}

// Recursive fibo
void test_14 (void)
{
    bint f = fibo_recursive(35);
    f.print();
}

void fibo()
{
    bint f0 = bint((char*)"0");
    bint f1 = bint((char*)"1");
    bint f2;
    
    int n = 2;
    while (1) 
    {
        f2 = f0 + f1;
        if (n == 4784969)
        {
            std::cout << "fibo " << n << std::endl;
            f2.print();
            break;
        }
        n++;
        f0 = f1 + f2;
        if (n == 4784969)
        {
            std::cout << "fibo " << n << std::endl;
            f0.print();
            break;
        }
        n++;
        f1 = f2 + f0;
        if (n == 4784969)
        {
            std::cout << "fibo " << n << std::endl;
            f1.print();
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
    bint x = bint((char*)"1234567887654321");
    x.print();
    bint low = x.low();
    low.print();

    bint high = x.high();
    high.print();
}

// Sub, same size
void test_17 (void)
{
    bint x = bint((char*)"4444444444444444");
    bint y = bint((char*)"1111116111111115");
    x.print();
    y.print();
    bint res = x.sub(y);
    res.print();
}

// Sub, shorter from longer
void test_18 (void)
{
    bint x = bint((char*)"4444444444444444");
    bint y = bint((char*)"1116111111115");
    x.print();
    y.print();
    bint res = x - y;
    res.print();
}

// Sub, longer from shorter (expect assertion failure)
void test_19 (void)
{
    bint x = bint((char*)"44444");
    bint y = bint((char*)"1111116111111115");
    x.print();
    y.print();
    bint res = x.sub(y);
    res.print();
}

// mul, the base case, one element in value, no overflow
void test_20 (void)
{
    bint x = bint((char*)"2");
    bint y = bint((char*)"3");
    x.print();
    y.print();
    bint res = x.mul(y);
    res.print();
}

// mul, the base case, one element in value, with overflow
void test_21 (void)
{
    bint x = bint((char*)"9");
    bint y = bint((char*)"9");
    x.print();
    y.print();
    bint res = x.mul(y);
    res.print();
}

// mul, testing spilt, two elements into 1
void test_22 (void)
{
    bint x = bint((char*)"1234");
    bint y = bint((char*)"5678");
    x.print();
    y.print();
    bint res = x.mul(y);
    res.print();
}


int main (int argc, char* argv[])
{
    uint64_t z = 10000000000000000000ULL; 

    test_22();

    return 0;
}

