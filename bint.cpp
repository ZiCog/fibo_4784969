#include <cstdint>
#include <iostream>
#include <cstring>
#include <ctype.h>
#include <cassert>

// uncomment to disable assert()
// #define NDEBUG
#include <cassert>

#include <bint.h>

char *strrev(char *str)
{
    if (!str || ! *str)
        return str;

    int i = strlen(str) - 1, j = 0;

    char ch;
    while (i > j)
    {
        ch = str[i];
        str[i] = str[j];
        str[j] = ch;
        i--;
        j++;
    }
    return str;
}


int allocCount = 0;

bint::bint ()
: value(0), width(0)
{
}

bint::bint (size_t width)
: width(width)
{
    value = new int64_t[width];
    allocCount++;
    bzero(value, width * sizeof value[0]);
}

bint::bint (const char* s)
{
    if (!s || ! *s)
    {
        return;
    }
    // FIXME Only one decimal digit per element here!
    width = strlen(s);
    value = new int64_t[width];
    allocCount++;
    bzero(value, width * sizeof value[0]);

    int i = 0;
    const char* r = s + strlen(s) - 1;

    while (r >= s)
    {
        value[i] = *r - '0';
        i++;
        r--;
    }
}

bint::bint (const bint& k) // copy constructor 
{
    value = k.value;
    width = k.width;
}

void bint::operator= (const bint& k)
{
    if (width != k.width)
    {
        width = k.width;
        delete[] value;    
        value = new int64_t[k.width];
    }
    memcpy(value, k.value, width * sizeof value[0]);
}

void bint::operator= (const char* s)
{
    width = strlen(s);
    delete[] value;    

    value = new int64_t[width];
    allocCount++;
    bzero(value, width * sizeof value[0]);

    int i = 0;
    const char* r = s + strlen(s) - 1;

    while (r >= s)
    {
        value[i] = *r - '0';
        i++;
        r--;
    }
}

bint::~bint ()
{
    delete[] value;
}

std::ostream& operator<<(std::ostream& os, const bint& b)  
{
    if (b.width == 0) 
    {
        os << "BIGNULL";
        return os;
    }  
    int i;
    int digits = 1;
    for (i = 0; i < b.width - 1 ; i ++)
    {
        os << std::setfill('0') << std::setw(digits) << b.value[i] << ", ";
    }
    os << std::setfill('0') << std::setw(digits) << b.value[i];

    os << " : ";

    for (i = b.width - 1; i >= 0; i--)
    {
        os << std::setfill('0') << std::setw(digits) << b.value[i];
    }

    return os;  
}  

void bint::grow ()
{
    int32_t newWidth = width * 2;   
    int64_t *newValue = new int64_t[newWidth];
    allocCount++;
    bzero(newValue, newWidth * sizeof newValue[0]);
    std::memcpy(newValue, value, width * sizeof newValue[0]);
    delete[] value;

    value = newValue;
    width = newWidth;
}

void bint::shrink (int newWidth)
{
    std::cout << "Shrinking width " << width << " to newWidth " << newWidth << std::endl;
    int64_t *newValue = new int64_t[newWidth];
    allocCount++;
    bzero(newValue, newWidth * sizeof newValue[0]);
    std::memcpy(newValue, value, newWidth * sizeof newValue[0]);
    delete[] value;

    value = newValue;
    width = newWidth;
}

const bint bint::low() const
{
    assert(width > 1);
    assert((width & 1) != 1);
 
    // Make a result half the size of this, containing low half of this
    int newWidth = this->width / 2;
    bint low(newWidth);
    std::memcpy(low.value, value, newWidth * sizeof low.value[0]);

    return low; 
}

const bint bint::high() const
{
    assert(width > 1);
    assert((width & 1) != 1);

    // Make a result half the size of this, containing high half of this
    int newWidth = this->width / 2;
    bint high(newWidth);
    std::memcpy(high.value, &value[newWidth], newWidth * sizeof high.value[0]);
    return high; 
}

bint bint::sum (const bint& n)
{
    // Ensure "a" operand is longer than "b" operand
    const bint *a = this;
    const bint *b = &n;
    if (n.width > width)
    {
        a = &n;
        b = this;
    }

    // Make a result of the same size as operand "a"
    bint result(a->width);

    int i;
    uint64_t sum = 0;
    uint64_t carry = 0;
    for (i = 0; i < a->width; i ++)
    {
        if (i < b->width)
        {
            sum = a->value[i] + b->value[i] + carry;
        }
        else
        {
            sum = a->value[i] + carry;
        }

        if (sum > LIMIT)
        {
            sum = sum - LIMIT - 1;
            carry = 1;
        }
        else
        {
            carry = 0;
        }
        result.value[i] = sum;
    }
    // If carry is set here we need more digits!
    if (carry)
    {
        result.grow();
        result.value[i] = 1;
    }
    return result; 
}

bint bint::sub (const bint& a)
{
    // Demand this operand is wider than the a operand
    if (this->width < a.width)
    {
        std::cout << "!!!!! this: " << *this << " a: " << a << std::endl; 
    }
    assert(this->width >= a.width);

    // Make a result of the same size as this
    bint result(this->width);

    int64_t diff = 0;
    int64_t borrow = 0;
    for (int i = 0; i < this->width; i ++)
    {
        if (i < a.width)
        {
            diff = this->value[i] - a.value[i] - borrow;
        }
        else
        {
            diff = this->value[i] - borrow;
        }
        if (diff < 0)
        {
            diff = diff + BASE;
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }
        result.value[i] = diff;
    }

    // If borrow is set here we have an error
    assert(borrow == 0);
    return result; 
}

bint bint::shift1 (int n)
{
    // Make a result of the required size
    bint result(this->width * 2);
    memmove(&result.value[n], &value[0], width * sizeof value[0]);
    return result; 
}

bint bint::shift2 (int n)
{
    // Make a result of the required size
    bint result(this->width * 4);
    memmove(&result.value[n], &value[0], width * sizeof value[0]);
    return result; 
}

/*
Karatsuba pseudo code from wikpedia:
https://en.wikipedia.org/wiki/Karatsuba_algorithm

    procedure karatsuba(num1, num2)
        if (num1 < 10) or (num2 < 10)
            return num1*num2

        /* calculates the size of the numbers * /
        m = min(size_base10(num1), size_base10(num2))
        m2 = floor(m/2)

        /* split the digit sequences in the middle * /
        high1, low1 = split_at(num1, m2)
        high2, low2 = split_at(num2, m2)

        /* 3 calls made to numbers approximately half the size * /
        z0 = karatsuba(low1, low2)
        z1 = karatsuba((low1 + high1), (low2 + high2))
        z2 = karatsuba(high1, high2)

        return (z2 * 10 ^ (m2 * 2)) + ((z1 - z2 - z0) * 10 ^ m2) + z0
*/

bint bint::mul (bint& a)
{
    // Ensure operands are same width.
    while (this->width > a.width)
    {
        a.grow();
    }
    while (a.width > this->width)
    {
        this->grow();
    }
    // Demand this operand is same width as a operand. FIXME: Is this required?
    assert(this->width == a.width);

    bint result(a.width * 2);

    // The base case, only one element in value, just do the multiply
    if (width == 1)
    {
        int64_t product = value[0] * a.value[0];
        if (product < BASE) {
            result.value[0] = product;
            result.value[1] = 0;
        }
        else
        {
            result.value[0] = (product % BASE);
            result.value[1] = (product / BASE);
        }
        return result; 
    }

    // Calculates the size of the numbers
    int m = (this->width);
    int m2 = m / 2;

    // Split the numbers in the middle
    bint high1 = this->high();
    bint low1 = this->low();
    bint high2 = a.high();
    bint low2 = a.low();

    bint z0 = low1 * low2;
    bint z2 = high1 * high2;
    bint s1 = low1 + high1;
    bint s2 = low2 + high2;
    bint z1 = s1 * s2;
    bint t1 = z1 - z2 - z0;
    bint t1Shifted = t1.shift1(m2);
    bint z2Shifted = z2.shift2(m);
    result = z2Shifted + t1Shifted + z0;
    result.shrink(m * 2);
    return result; 
}
