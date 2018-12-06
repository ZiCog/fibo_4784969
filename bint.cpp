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
    std::cout << "Constructor: " << std::endl;
    value = new int64_t[WIDTH];
    width = WIDTH;
    bzero(value, width * sizeof value[0]);
}

bint::bint (int64_t x)
{
    value = new int64_t[WIDTH];
    width = WIDTH;
    bzero(value, width * sizeof value[0]);
    value[0] = x;

}

bint::bint (const char* s)
{
    if (!s || ! *s)
    {
        return;
    }
    std::cout << "Constructed with string: " << s << std::endl;
    // FIXME Only one decimal digit per element here!
    width = strlen(s);
    value = new int64_t[width];
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
    std::cout << "operator=:  bint: " << k << std::endl;
    width = k.width;
    delete[] value;    
    value = new int64_t[k.width];
    memcpy(value, k.value, width * sizeof value[0]);
}

void bint::operator= (const char* s)
{
    std::cout << "operator=: string: " << s << std::endl;
    width = strlen(s);
    delete[] value;    
    value = new int64_t[width];
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
    std::cout << "Destructor: " << std::endl;
    std::cout << "Deleting: " << std::endl;
    delete[] value;
}

std::ostream& operator<<(std::ostream& os, const bint& b)  
{  
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

const bint bint::low() const
{
    assert(width > 1);
    assert((width & 1) != 1);
 
    // Make a result half the size of this, containing low half of this
    int newWidth = this->width / 2;
    bint low;
    low.resize(newWidth);
    std::memcpy(low.value, value, newWidth * sizeof low.value[0]);
    return low; 
}

const bint bint::high() const
{
    assert(width > 1);
    assert((width & 1) != 1);

    // Make a result half the size of this, containing high half of this
    int newWidth = this->width / 2;
    bint high;
    high.resize(newWidth);
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
    bint result;
    result.resize(a->width);

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
    assert(this->width >= a.width);

    // Make a result of the same size as this
    bint result;
    result.resize(this->width);

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
    bint result;
    result.resize(this->width * 2);
    memmove(&result.value[n], &value[0], width * sizeof value[0]);
    return result; 
}

bint bint::shift2 (int n)
{
    // Make a result of the required size
    bint result;
    result.resize(this->width * 2);
    memmove(&result.value[n], &value[0], width * sizeof value[0]);
    return result; 
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


bint bint::mul (const bint& a)
{
    std::cout << "Enter: ******************" <<  *this << ", " << a << std::endl; 
    bint result;

    // Demand this operand is same width as a operand. FIXME: Is this required?
    assert(this->width == a.width);

    // The base case, only one element in value, just do the multiply
    if (width == 1)
    {
        result.grow();
        int64_t product = value[0] * a.value[0];
        if (product < BASE) {
            result.value[0] = product;
            result.value[1] = 0;
        }
        else
        {
            result.value[0] = (product % 10);
            result.value[1] = (product / 10);
        }
        std::cout << "Exit: ******************" << std::endl; 
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

    std::cout << "high1: " << high1 << std::endl; 
    std::cout << "low1: " << low1 << std::endl; 
    std::cout << "high2: " << high2 << std::endl; 
    std::cout << "low2: " << low2 << std::endl; 

    bint z0 = low1.mul(low2);
    std::cout << "z0: " << z0 << std::endl; 

    bint z2 = high1.mul(high2);
    std::cout << "z2: " << z2 << std::endl; 

    // z1 = karatsuba((low1 + high1), (low2 + high2))
    bint s1 = low1 + high1;
//    std::cout << "s1: " << s1 << std::endl; 

    bint s2 = low2 + high2;
//    std::cout << "s2: " << s2 << std::endl; 

    bint z1 = s1.mul(s2);
//    std::cout << "z1: " << z1 << std::endl; 

    bint t1 = z1 - z2 - z0;
//    std::cout << "t1: " << t1 << std::endl; 

    bint t1Shifted = t1.shift1(m2);
//    std::cout << "t1Shifted: " << t1Shifted << std::endl; 

    bint z2Shifted = z2.shift2(m);
//    std::cout << "z2Shifted: " << z2Shifted << std::endl; 

    result = z2Shifted + t1Shifted + z0;
//    std::cout << "result: " << result << std::endl; 

    std::cout << "Exit: ******************" << std::endl; 

    return result; 
}














