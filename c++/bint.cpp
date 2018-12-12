#include <cstdint>
#include <iostream>
#include <cstring>
#include <ctype.h>
#include <cassert>

// uncomment to disable assert()
// #define NDEBUG
#include <cassert>
#include <bint.h>

bint::bint ()
: value(0), width(0)
{
}

bint::bint (size_t width)
: width(width)
{
    value = new uint64_t[width + 1];
    bzero(value, width * sizeof value[0]);
}

uint64_t parseDigits(const char* s, int len)
{
    uint32_t num = 0;
    for (int i = 0; i < len; i++)
    {
        num = num * 10 + *s++ - '0';
    }
    return num;
}

bint::bint (const char* s)
{
    if (!s || ! *s)
    {
        width = 0;
        value = 0;
        return;
    }

    int d1 = strlen(s) / DIGITS;
    int d2 = strlen(s) % DIGITS;

    width = d1;
    if (d2) width++;

    value = new uint64_t[width];
    bzero(value, width * sizeof(uint64_t));

    int w = width - 1;
    if(d2)
    {
        value[w--] = parseDigits(s, d2);
        s = s + d2;
    }

    while (w >= 0)
    {
        value[w--] = parseDigits(s, DIGITS);
        s = s + DIGITS;
    }
}

bint::bint (const bint& k) // copy constructor 
{
    width = k.width;
    value = new uint64_t[k.width];
    memcpy(value, k.value, width * sizeof value[0]);
}

void bint::operator= (const bint& k)
{
    if (width != k.width)
    {
        width = k.width;
        delete[] value;    
        value = new uint64_t[k.width];
    }
    memcpy(value, k.value, width * sizeof value[0]);
}


void bint::operator= (const char* s)
{
    width = strlen(s);
    delete[] value;    

    value = new uint64_t[width];
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
        os << "BINTNULL";
    }
    else
    {
        for (int i = b.width - 1; i >= 0; i--)
        {
            os << std::setfill('0') << std::setw(DIGITS) << b.value[i];
        }
    }
    return os;  
}  

void bint::grow ()
{
    int32_t newWidth = width + 1;   
    uint64_t *newValue = new uint64_t[newWidth];
    bzero(newValue, newWidth * sizeof newValue[0]);
    std::memcpy(newValue, value, width * sizeof newValue[0]);
    delete[] value;

    value = newValue;
    width = newWidth;
}

const bint bint::low(int mid) const
{
    assert(width > 1);
    assert(mid < width);
 
    // Make a result half the size of this, containing low half of this
    bint low(mid);
    std::memcpy(low.value, value, mid * sizeof low.value[0]);
    return low; 
}

const bint bint::high(int mid) const
{
    assert(width > 1);
    assert(mid < width);

    // Make a result half the size of this, containing high half of this
    bint high(width - mid);
    std::memcpy(high.value, &value[mid], (width - mid) * sizeof high.value[0]);
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
    bint sum(a->width);

    int i;
    uint64_t s = 0;
    uint64_t carry = 0;
    for (i = 0; i < a->width; i ++)
    {
        if (i < b->width)
        {
            s = a->value[i] + b->value[i];
        }
        else
        {
            s = a->value[i];
        }
        s += carry;


        if (s >= BASE)
        {
            s -= BASE;
            carry = 1;
        }
        else
        {
            carry = 0;
        }
        sum.value[i] = s;
    }

    // If carry is set here we need more digits!
    if (carry)
    {
        sum.grow();
        sum.value[i] = 1;
    }
    return sum; 
}

bint bint::sub (const bint& b)
{
    // Demand this operand is wider than the a operand
    if (this->width < b.width)
    {
        std::cout << "!!!!! this: " << *this << " b: " << b << std::endl; 
    }
    assert(this->width >= b.width);

    // Make a result of the same size as this
    bint difference(this->width);

    uint64_t borrow = 0;
    for (int i = 0; i < this->width; i ++)
    {
        uint64_t x = this->value[i];
        uint64_t y = (i < b.width) ? b.value[i] : 0;

        if (borrow) {
            x--;
        }

        uint64_t d;
        if (x < y)
        {
            d = x + BASE - y;
            borrow = 1;
        }
        else
        {
            d = x - y;
            borrow = 0;
        }
        difference.value[i] = d;
    }
    return difference;
}

bint bint::shift (int n)
{
    // Make a result of the required size
    bint result(this->width + n);
    memmove(&result.value[n], &value[0], width * sizeof value[0]);
    return result; 
}

bint bint::simpleMul (uint64_t k) const
{
    bint product(width);
    uint64_t carry = 0;
    int i = 0;
    for (i = 0; i < width; i++) {
        uint64_t p = value[i] * k + carry;
        if (p < BASE)
        {
            product.value[i] = p;
            carry = 0;
        }
        else
        {
            carry = p / BASE;
            product.value[i] = p % BASE;
        }
    }
    if (carry)
    {
        product.grow();
        product.value[i] = carry;
    }
    return product;
}

bint bint::mul (const bint& b)
{
    // The base case(s), only one element in value, just do the multiply
    if (width == 1)
    {
        return b.simpleMul(value[0]);
    }
    if (b.width == 1)
    {
        return simpleMul(b.value[0]);
    }
    // Calculates the size of the numbers
    int m = (this->width);
    int m2 = m / 2;

    // Split the numbers in the middle
    bint high1 = this->high(m2);
    bint low1 = this->low(m2);
    bint high2 = b.high(m2);
    bint low2 = b.low(m2);

    // Do da karatsaba shuffle, yabba dabba do.
    bint z0 = low1 * low2;
    bint z1 = (low1 + high1) * (low2 + high2);
    bint z2 = high1 * high2;

    bint s1 = z1 - z2;
    bint s2 = s1 - z0;

    return  z2.shift(m2 * 2) + s2.shift(m2) + z0;
}
