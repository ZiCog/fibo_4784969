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

bint::bint ()
{
    value = new int64_t[WIDTH];
    width = WIDTH;
    bzero(value, width * sizeof value[0]);
    refCount = 1;
}

bint::bint (int64_t x)
{
    value = new int64_t[WIDTH];
    width = WIDTH;
    bzero(value, width * sizeof value[0]);
    value[0] = x;
    refCount = 1;
}

bint::bint (char* s)
{
    if (!s || ! *s)
    {
        bint();
        return;
    }
    std::cout << "Constructed with string: " << s << std::endl;
    // FIXME Only one decimal digit per element here!
    value = new int64_t[WIDTH];
    width = WIDTH;
    bzero(value, width * sizeof value[0]);

    int i = 0;
    char* r = s + strlen(s) - 1;

    while (r >= s)
    {
        value[i] = *r - '0';
        i++;
        if ((i == width) && (r > s))
        {
            grow();
        }
        r--;
    }
    refCount = 1;
}

bint::bint (const bint& k) // copy constructor 
{
    value = k.value;
    width = k.width;
    refCount = k.refCount + 1;
}

bint::~bint ()
{
    refCount -= 1;
    if (refCount == 0)
    {
        delete[] value;
    }
}

void bint::print ()
{
    int i;
    int digits = 1;
    std::cout << "Elements: " << std::endl;
    for (i = 0; i < width - 1 ; i ++)
    {
        std::cout << std::setfill('0') << std::setw(digits) << value[i] << ", ";
    }
    std::cout << std::setfill('0') << std::setw(digits) << value[i] << std::endl;

    std::cout << "As number: " << std::endl;
    for (i = width - 1; i >= 0; i--)
    {
        std::cout << std::setfill('0') << std::setw(digits) << value[i];
    }
    std::cout << std::endl;
    std::cout << std::setfill('*') << std::setw(80) << "*" << std::endl;
}

void bint::grow ()
{
    int32_t newWidth = width * 2;   
    std::cout << "Grow:" << newWidth << std::endl;
    int64_t *newValue = new int64_t[newWidth];
    bzero(newValue, newWidth * sizeof newValue[0]);
    std::memcpy(newValue, value, width * sizeof newValue[0]);
    delete[] value;
    value = newValue;
    width = newWidth;
}

void bint::resize (int32_t newWidth)
{
    int64_t *newValue = new int64_t[newWidth];
    bzero(newValue, newWidth * sizeof newValue[0]);
    std::memcpy(newValue, value, width * sizeof newValue[0]);
    delete[] value;
    value = newValue;
    width = newWidth;
}

bint& bint::swap(bint& a)
{
    int64_t* valueTemp = value;
    int32_t  widthTemp = width;
    value = a.value;
    width = a.width;
    a.value = valueTemp;
    a.width = widthTemp;
    return a;
}

bint& bint::low()
{
    assert(width > 1);
    assert((width & 1) != 1);
 
    // Make a result half the size of this, containing low half of this
    int newWidth = this->width / 2;
    bint *low = new bint();
    low->resize(newWidth);
    std::memcpy(low->value, value, newWidth * sizeof low->value[0]);
    return *low; 
}

bint& bint::high()
{
    assert(width > 1);
    assert((width & 1) != 1);

    // Make a result half the size of this, containing high half of this
    int newWidth = this->width / 2;
    bint *high = new bint();
    high->resize(newWidth);
    std::memcpy(high->value, &value[newWidth], width * sizeof high->value[0]);
    return *high; 
}

bint& bint::sum (const bint& n)
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
    bint *result = new bint();
    result->resize(a->width);

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
        result->value[i] = sum;
    }
    // If carry is set here we need more digits!
    if (carry)
    {
        result->grow();
        result->value[i] = 1;
    }
    return *result; 
}

bint& bint::sub (const bint& a)
{
    // Demand this operand is wider than the a operand
    assert(this->width >= a.width);

    // Make a result of the same size as this
    bint *result = new bint();
    result->resize(this->width);

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
        result->value[i] = diff;
    }

    // If borrow is set here we have an error
    assert(borrow == 0);
    return *result; 
}

/*
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


bint& bint::mul (const bint& a)
{
    // Demand this operand is same width as a operand. FIXME: Is this required?
    assert(this->width == a.width);

    // The base case, only one element in value, just do the multiply
    if (width == 1)
    {
        bint *result = new bint();
        int64_t product = value[0] * a.value[0];
        if (product < BASE) {
            result->value[0] = product;
        }
        else
        {
            result->grow();
            result->value[0] = (product % 10);
            result->value[1] = (product / 10);
        }
        return *result; 
    }

    assert (1 == 2);



}














