#include <cstdint>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <ctype.h>
#include <cassert>

//#define BASE 10000000000000000000ULL
//#define BASE   1000000000000000000ULL
#define BASE   10ULL
#define LIMIT (BASE - 1)
#define WIDTH 1

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

    const bint high() const;

    const bint low() const;

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

    bint shift1 (int n);

    bint shift2 (int n);

    bint mul (bint& a);

    bint operator* (bint& n)
    {
        return (*this).mul(n);        
    }

    friend std::ostream& operator<<(std::ostream& os, const bint& b);
private:
    int64_t* value;
    int32_t  width;
};
