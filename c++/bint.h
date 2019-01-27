#ifndef BINT_H
#define BINT_H

#include <cstring>
#include <iomanip>
#include <iostream>
#include <algorithm>



#ifdef USE_ASYNC
#include <future>
#endif

#ifdef _OPENMP
#include <omp.h>
#endif

// Uncomment to disable assert()
//#define NDEBUG
#include <cassert>

#ifdef TEST
#define private public
#endif

#define bzero(p, size)     (void)memset((p), 0, (size))

typedef uint64_t bintel_t;

// Provide pow as constexpr as clang does not allow pow in constexpr
constexpr double cexp_pow(double base, int exp) {
    if (exp < 0.0) {
        base = 1.0 / base;
        exp = -exp;
    }
    double result = 1.0;
    for (int i = 0; i < exp; ++i) {
        result = result * base;
    }
    return result;
}

constexpr int DIGITS = 9;                       // Decimal digits in each big integer array element.
constexpr bintel_t BASE = cexp_pow(10, DIGITS);

constexpr int STACK_VALUE_SIZE = 128;
constexpr int ON2_CUTOFF = 53;
constexpr int CARRY_DELAY = 16;

class bint {
  public:
    inline bint() : value(0), width(0), parent(0) {}

    bint(size_t width) : width(width), parent(0) {
        value = allocate(width);
        bzero(value, width * sizeof value[0]);
    }

    inline bint(const char *s) {
        if (!s || !*s) {
            width = 0;
            value = 0;
            return;
        }

        int d1 = strlen(s) / DIGITS;
        int d2 = strlen(s) % DIGITS;

        width = d1;
        if (d2)
            width++;

        value = allocate(width);
        parent = 0;
        bzero(value, width * sizeof(bintel_t));

        int w = width - 1;
        if (d2) {
            value[w--] = parseDigits(s, d2);
            s = s + d2;
        }

        while (w >= 0) {
            value[w--] = parseDigits(s, DIGITS);
            s = s + DIGITS;
        }
    }

    // copy constructor
    inline bint(const bint &k) {
        width = k.width;
        value = allocate(k.width);
        parent = 0;
        memcpy(value, k.value, width * sizeof value[0]);
    }

    // Move constructor.
    bint(bint &&other) {
        static_assert(std::is_move_constructible<bint>::value,
                      "Not move constructible.");
        width = other.width;
        parent = other.parent;
        if (other.value == other.valueOnstack) {
            memcpy(valueOnstack, other.valueOnstack, sizeof(valueOnstack));
            value = valueOnstack;
        } else {
            value = other.value;
        }

        other.value = nullptr;
        other.width = 0;
        other.parent = 0;
    }

    // Move assignment operator.
    bint &operator=(bint &&other) {
        static_assert(std::is_move_assignable<bint>::value,
                      "Not move assignable.");

        if (this != &other) {
            width = other.width;
            parent = other.parent;
            if (other.value == other.valueOnstack) {
                memcpy(valueOnstack, other.valueOnstack, sizeof(valueOnstack));
                value = valueOnstack;
            } else {
                deallocate(value);
                value = other.value;
            }

            other.value = nullptr;
            other.width = 0;
            other.parent = 0;
        }
        return *this;
    }

    inline ~bint() {
        if (parent == 0) {
            // assert(value != 0);
            deallocate(value);
        }
    }

    inline void operator=(const bint &k) {
        if (width != k.width) {
            width = k.width;
            if (value != 0) {
                deallocate(value);
            }
            value = allocate(k.width);
        }
        memcpy(value, k.value, width * sizeof value[0]);
    }

    bool operator==(const bint &rhs) const {
        if (rhs.width != width) {
            return false;
        }
        if (memcmp(this->value, rhs.value, width * sizeof(bintel_t)) != 0) {
            return false;
        }
        return true;
    }

    bool operator!=(const bint &rhs) const { return !(*this == rhs); }

    inline bint operator+(const bint &b) const {
        // Ensure lhs operand is longer than rhs operand
        const bint *a = this;
        if (b.width > width) {
            return b + *this;
        }

        // Make a result of the same size as operand "a" with room for overflow
        bint sum(a->width + 1);
        sum.width--;

        int32_t i = 0;
        bintel_t s = 0;
        bintel_t carry = 0;
        while (i < b.width) {
            s = a->value[i] + b.value[i] + carry;
            carry = (s >= BASE);
            s -= BASE * carry;
            sum.value[i] = s;
            i++;
        }
        while (i < a->width) {
            if (!carry) break;
            s = a->value[i] + carry;
            carry = (s >= BASE);
            s -= BASE * carry;
            sum.value[i] = s;
            i++;
        }
        // If carry is set here we need more digits!
        if (carry) {
            sum.value[i] = 1;
            sum.width++;
        }
        while ((sum.width > 1) && (sum.value[sum.width - 1] == 0)){
            sum.width--;
        }
        if (sum.width > 1) {
            assert((sum.value[sum.width - 1] != 0));
        }
        return sum;
    }

    inline bint& operator++()
    {
        bintel_t carry = 1;
        bintel_t s;
        // Actual increment takes place here
        for (int i = 0; i < this->width; i++) {
            s = this->value[i];
            s += carry;
            carry = (s >= BASE);
            s -= BASE * carry;
            this->value[i] = s;
            if (carry == 0) {
                break;
            }
        }
        assert ((carry != 0) && "operator++ has overflowed."); 
        return *this;
    }

    inline bint  operator++(int)
    {
        bint tmp(*this); // copy
        operator++(); // Pre-increment
        return tmp;   // Return old value
    }

    inline bint & operator--()
    {
        // Actual decrement takes place here
        return *this;
    }

    inline bint  operator--(int)
    {
        bint tmp(*this); // copy
        operator--(); // Pre-decrement
        return tmp;   // Return old value
    }

    inline bint operator-(const bint &b) const {
        assert((this->width >= b.width) && "operator-() requires operand a wider or same as operand b");

        // Make a result of the same size as this
        bint difference(this->width);

        bintel_t borrow = 0;
        int32_t i = 0;
        while (i < b.width) {
            bintel_t t = this->value[i] - borrow;
            borrow = int64_t(t) < int64_t(b.value[i]);
            difference.value[i] = t + (BASE * borrow) - b.value[i];
            i++;
        }
        while (i < this->width) {
            bintel_t t = this->value[i] - borrow;
            borrow = int64_t(t) < 0;
            difference.value[i] = t + (BASE * borrow);
            i++;
        }
        assert((borrow == 0) && "operator-() requires operand b less than operand a");
        while ((difference.width > 1) && (difference.value[difference.width - 1] == 0)){
            difference.width--;
        }

        if (difference.width > 1) {
            assert((difference.value[difference.width - 1] != 0));
        }
        return difference;
    }

    inline bint operator*(const bint &b) const {
        // The base case(s), only one element in each value, just do the
        // multiply
        if ((width == 1) && (b.width == 1)) {
            bint product(2);

            bintel_t p = value[0] * b.value[0];
            if (p < BASE) {
                product.value[0] = p;
                product.width--;
            } else {
                product.value[0] = p % BASE;
                product.value[1] = p / BASE;
            }
            return product;
        }
        if (std::min(width, b.width) <= 49) {
            return o2nMul(b);
        }

        // Calculates the size of the numbers
        int m = (this->width);
        int m2 = m / 2;

        // Split the numbers in the middle
        const bint high1 = this->high(m2);
        const bint low1 = this->low(m2);
        const bint high2 = b.high(m2);
        const bint low2 = b.low(m2);

        bint z0;
        bint z1;
        bint z2;
        // Do da karatsaba shuffle, yabba dabba do.

#ifdef USE_ASYNC
        if ((low1.width > (1<<11)) && (low2.width > (1<<11))) { 
            auto f0 = std::async([low1, low2]() {return(low1 * low2);});
            auto f2 = std::async([high1, high2](){return(high1 * high2);});
            z1 = (low1 + high1) * (low2 + high2);
            z2 = f2.get();
            z0 = f0.get();
        } else {
            z0 = low1 * low2;
            z1 = (low1 + high1) * (low2 + high2);
            z2 = high1 * high2;
        }
#elif USE_OMP
        if ((low1.width > (1<<8)) && (low2.width > (1<<8))) { 
            #pragma omp task default(shared)
            z0 = low1 * low2;

            #pragma omp task default(shared)
            z1 = (low1 + high1) * (low2 + high2);

            #pragma omp task default(shared)
            z2 = high1 * high2;

            #pragma omp taskwait
        } else {
            z0 = low1 * low2;
            z1 = (low1 + high1) * (low2 + high2);
            z2 = high1 * high2;
        }
#else
        z0 = low1 * low2;
        z1 = (low1 + high1) * (low2 + high2);
        z2 = high1 * high2;
#endif

        const bint s2 = z1 - z2 - z0;

        bint result = shiftAndAdd(z2, s2, z0, m2 * 2, m2);
        if (result.width > 1) {
            assert((result.value[result.width - 1] != 0));
        }
        return result;
    }

    inline friend std::ostream &operator<<(std::ostream &os, const bint &b) {
        if (b.width == 0) {
            os << "BINTNULL";
        } else {
            os << b.value[b.width - 1];
            for (int i = b.width - 2; i >= 0; i--) {
                os << std::setfill('0') << std::setw(DIGITS) << b.value[i];
            }
        }
        return os;
    }

    int32_t getWidth() {
        return this->width;
    }

  private:
    inline bintel_t parseDigits(const char *s, int len) {
        bintel_t num = 0;
        for (int i = 0; i < len; i++) {
            num = num * 10 + *s++ - '0';
        }
        return num;
    }

    inline bintel_t *allocate(size_t n) {
        if (n <= STACK_VALUE_SIZE) {
            value = valueOnstack;
            return valueOnstack;
        } else {
            value = nullptr;
            return new bintel_t[n];
        }
    }

    inline void deallocate(bintel_t *d) {
        if (value != valueOnstack) {
            delete[] d;
        }
    }

    inline const bint low(int32_t mid) const {
        assert(width > 1);
        assert(mid < width);

        bint low;
        low.value = value;
        low.width = mid;
        low.parent = this;
        return low;
    }

    inline const bint high(int32_t mid) const {
        assert(width > 1);
        assert(mid < width);

        bint high;
        high.value = &value[mid];
        high.width = width - mid;
        high.parent = this;
        return high;
    }

    // Calculate:  a.shift(n) + b.shift(n/2) + c;
    inline bint shiftAndAdd(const bint &a, const bint &b, const bint &c,
                            int aShift, int bShift) const {
        assert(aShift >= bShift);

        // Make a result big enough with room for overflow
        int32_t newWidth;
        if ((a.width + aShift) > (b.width + bShift)) {
            newWidth = a.width + aShift + 1;
        } else {
            newWidth = b.width + bShift + 1;
        }
        bint result(newWidth);

        assert(result.width >= a.width + aShift);
        assert(result.width >= b.width + bShift);

        // Move c into the result
        memcpy(result.value, c.value, c.width * sizeof(bintel_t));
        bzero(result.value + c.width,
              (result.width - c.width) * sizeof(bintel_t));

        // Add b into the result with offset bShift
        bintel_t s = 0;
        bintel_t carry = 0;
        bintel_t *bPtr = b.value;
        bintel_t *rPtr = result.value + bShift;
        int32_t i = 0;
        while (i < b.width) {
            s = *rPtr + *bPtr + carry;
            carry = (s >= BASE);
            s -= BASE * carry;
            *rPtr = s;
            bPtr++;
            rPtr++;
            i++;
        }
        while (i < result.width - bShift - 1) {
            if (!carry) break;
            s = (*rPtr) + carry;
            carry = (s >= BASE);
            s -= BASE * carry;
            *rPtr = s;
            rPtr++;
            i++;
        }
        // Propagate carry       // FIXME: There are two carries going on here.
        // This is not right yet.
        *rPtr++ += carry;

        // Add a into the result with offset aShift
        s = 0;
        carry = 0;
        i = 0;
        bintel_t *aPtr = a.value;
        rPtr = result.value + aShift;
        while (i < a.width) {
            s = *rPtr + *aPtr + carry;
            carry = (s >= BASE);
            s -= BASE * carry;
            *rPtr = s;
            aPtr++;
            rPtr++;
            i++;
        }
        while (i < result.width - aShift - 1) {
            if (!carry) break;
            s = *rPtr + carry;
            carry = (s >= BASE);
            s -= BASE * carry;
            *rPtr = s;
            rPtr++;
            i++;
        }
        // If carry is set here we need more digits!        // FIXME: There are
        // two carries going on here. This is not right yet.
        if (carry) {
            *rPtr = 1;
        } else {
            result.width--;
        }

        return result;
    }

    inline bint o2nMul(const bint &b) const {
        assert((this->width > 0 && b.width > 0) &&
               "o2nMul() requires operands wider than 0");

        bint x = bint(this->width + b.width + 1);
        x.width = this->width + b.width - 1;
        for (int32_t i = 0; i < this->width; ++i) {
            for (int32_t j = 0; j < b.width; ++j) {
                x.value[i + j] += this->value[i] * b.value[j];
            }
//            if ((this->width - i) % CARRY_DELAY == 1) {
            if (((this->width - i) & 0x1f) == 1) {
                for (int32_t k = 0; k <= x.width; ++k) {
                    if (x.value[k] >= BASE) {
                        const bintel_t c = x.value[k] / BASE;
                        x.value[k] %= BASE;
                        x.value[k + 1] += c;
                    }
                }
            }
        }
        if (x.value[x.width]) {
            ++x.width;
        }
        return x;
    }

  private:
    bintel_t *value;
    bintel_t valueOnstack[STACK_VALUE_SIZE];
    int32_t width;
    const bint *parent;
};
#endif // BINT_H
