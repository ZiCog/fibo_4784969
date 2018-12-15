#ifndef BINT_H
#define BINT_H

#include <cassert>
#include <cstdint>
#include <cstring>
#include <ctype.h>
#include <iomanip>
#include <iostream>
#include <math.h>

// Uncomment to disable assert()
//#define NDEBUG
#include <cassert>

constexpr int DIGITS = 9; // Decimal digits in each big integer array element.
constexpr uint64_t BASE = pow(10, DIGITS);
constexpr uint64_t LIMIT = BASE - 1;

#if DEBUG
uint64_t allocWithWidth = 0;
uint64_t allocCopy = 0;
uint64_t allocString = 0;
uint64_t allocEquals = 0;
uint64_t allocEqualsString = 0;
uint64_t allocHigh = 0;
uint64_t allocLow = 0;
uint64_t allocShift = 0;
uint64_t allocGrow = 0;
uint64_t allocBytes = 0;
#endif

class bint {
  public:
    inline bint() : value(0), width(0), parent(0) {}

    bint(size_t width) : width(width), parent(0) {
        value = new uint64_t[width];
        bzero(value, width * sizeof value[0]);
#if DEBUG
        allocBytes += width + 1;
        allocWithWidth++;
#endif
    }

    inline uint64_t parseDigits(const char *s, int len) {
        uint64_t num = 0;
        for (int i = 0; i < len; i++) {
            num = num * 10 + *s++ - '0';
        }
        return num;
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

        value = new uint64_t[width];
        parent = 0;
#if DEBUG
        allocString++;
        allocBytes += width;
#endif
        bzero(value, width * sizeof(uint64_t));

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
        value = new uint64_t[k.width];
        parent = 0;
        memcpy(value, k.value, width * sizeof value[0]);
#if DEBUG
        allocBytes += k.width;
        allocCopy++;
#endif
    }

    inline ~bint() {
        if (parent == 0) {
            delete[] value;
        }
    }

    inline void operator=(const bint &k) {
        if (width != k.width) {
            width = k.width;
            delete[] value;
            value = new uint64_t[k.width];
#if DEBUG
            allocBytes += k.width;
            allocEquals++;
#endif
        }
        memcpy(value, k.value, width * sizeof value[0]);
    }

    void operator=(const char *s) {
        width = strlen(s);
        delete[] value;

        value = new uint64_t[width];
#if DEBUG
        allocEqualsString++;
        allocBytes += width;
#endif
        bzero(value, width * sizeof value[0]);

        int i = 0;
        const char *r = s + strlen(s) - 1;

        while (r >= s) {
            value[i] = *r - '0';
            i++;
            r--;
        }
    }

    inline const bint low(uint64_t mid) const {
        assert(width > 1);
        assert(mid < width);

        bint low;
        low.value = value;
        low.width = mid;
        low.parent = this;
        return low;
    }

    inline const bint high(uint64_t mid) const {
        assert(width > 1);
        assert(mid < width);

        bint high;
        high.value = &value[mid];
        high.width = width - mid;
        high.parent = this;
        return high;
    }

    inline bint shift(int n) const {
        // Make a result of the required size
        bint result(this->width + n);
        memmove(&result.value[n], &value[0], width * sizeof value[0]);
#if DEBUG
        allocShift++;
#endif
        return result;
    }

    void growByOne() {
        uint64_t newWidth = width + 1;
        uint64_t *newValue = new uint64_t[newWidth];
        bzero(newValue, newWidth * sizeof newValue[0]);
        std::memcpy(newValue, value, width * sizeof newValue[0]);
        delete[] value;
#if DEBUG
        allocGrow++;
        allocBytes += newWidth;
#endif
        value = newValue;
        width = newWidth;
    }

    inline bint operator+(const bint &n) const {
        // Ensure "a" operand is longer than "b" operand
        const bint *a = this;
        const bint *b = &n;
        if (n.width > width) {
            a = &n;
            b = this;
        }

        // Make a result of the same size as operand "a"
        bint sum(a->width);

        uint64_t i = 0;
        uint64_t s = 0;
        uint64_t carry = 0;
        uint64_t *aPtr = a->value;
        uint64_t *bPtr = b->value;
        uint64_t *sPtr = sum.value;
        while (i < b->width) {
            s = *aPtr++ + *bPtr++ + carry;
            carry = (s >= BASE);
            s -= BASE * carry;
            *sPtr++ = s;
            i++;
        }
        while (i < a->width) {
            s = *aPtr++ + carry;
            carry = (s >= BASE);
            s -= BASE * carry;
            *sPtr++ = s;
            i++;
        }
        // If carry is set here we need more digits!
        if (carry) {
            sum.growByOne();
            sum.value[i] = 1;
        }
        return sum;
    }

    inline bint operator-(const bint &b) const {
        // Demand this operand is wider than the a operand
        if (this->width < b.width) {
            std::cout << "!!!!! this width: " << this->width << " is less than b width: " << b.width << std::endl;
            std::cout << "!!!!! this: " << *this << " b: " << b << std::endl;
        }
        assert(this->width >= b.width);

        // Make a result of the same size as this
        bint difference(this->width);

        uint64_t borrow = 0;
        uint64_t i = 0;
        uint64_t *aPtr = this->value;
        uint64_t *bPtr = b.value;
        uint64_t *dPtr = difference.value;
        while (i < b.width) {
            *aPtr -= borrow;
            borrow = (*aPtr < *bPtr);
            *dPtr = *aPtr + (BASE * borrow) - *bPtr;
            dPtr++;
            aPtr++;
            bPtr++;
            i++;
        }
        while (i < this->width) {
            *aPtr -= borrow;
            borrow = (*aPtr < 0); // FIXME: WTF? unsigned less than zero ?!
            *dPtr = *aPtr + (BASE * borrow);
            dPtr++;
            aPtr++;
            i++;
        }
        // If there is a borrow here we have a problem, we can't handle negative numbers
        assert(borrow == 0);

        return difference;
    }

    inline bint simpleMul(uint64_t k) const {
        bint product(width);
        uint64_t carry = 0;
        uint64_t i = 0;
        for (i = 0; i < width; i++) {
            uint64_t p = value[i] * k + carry;
            if (p < BASE) {
                product.value[i] = p;
                carry = 0;
            } else {
                carry = p / BASE;
                product.value[i] = p % BASE;
            }
        }
        // If carry we need more digits
        if (carry) {
            product.growByOne();
            product.value[i] = carry;
        }
        return product;
    }

    inline bint operator*(const bint &b) const {
        // The base case(s), only one element in value, just do the multiply
        if (width == 1) {
            return b.simpleMul(value[0]);
        }
        if (b.width == 1) {
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

        bint s2 = z1 - z2 - z0;

        return z2.shift(m2 * 2) + s2.shift(m2) + z0;
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

  private:
    uint64_t *value;
    uint64_t width;
    const bint *parent;
};



#endif // BINT_H
