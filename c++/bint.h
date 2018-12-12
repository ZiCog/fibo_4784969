#include <cstdint>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <ctype.h>
#include <cassert>
#include <math.h>

constexpr int DIGITS = 9;                  // Decimal digits in each big integer array element.
constexpr uint64_t BASE = pow(10, DIGITS);
constexpr uint64_t LIMIT = BASE - 1;

class bint
{
public:
    bint ();

    bint (size_t width);

    bint (const char* s);

    bint (const bint& k); // copy constructor 

    void operator= (const bint& k);

    void operator= (const char* s);

    ~bint ();

    void grow ();

    void shrink (int newWidth);

    const bint high(int mid) const;

    const bint low(int mid) const;

    bint sum (const bint& n);

    bint operator+ (const bint& n)
    {
        return (*this).sum(n);        
    }

    bint sub (const bint& a);

    bint operator- (const bint& n)
    {
        return (*this).sub(n);        
    }

    bint shift (int n);

    bint simpleMul (uint64_t k);

    bint mul (bint& a);

    bint operator* (bint& n)
    {
        return (*this).mul(n);        
    }

    friend std::ostream& operator<<(std::ostream& os, const bint& b);
private:
    uint64_t* value;
    int32_t  width;
};
