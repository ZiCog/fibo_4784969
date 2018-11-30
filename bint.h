#include <cstdint>
#include <iostream>
#include <cstring>
#include <ctype.h>
#include <cassert>

#define LIMIT 9
#define WIDTH 4

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

class bint
{
public:
    bint ()
    {
        value = new int64_t[WIDTH];
        width = WIDTH;
        bzero(value, width * sizeof value[0]);
    }

    bint (int64_t x)
    {
        value = new int64_t[WIDTH];
        width = WIDTH;
        bzero(value, width * sizeof value[0]);
        value[0] = x;
    }

    bint (char* s)
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
    }

    bint (const bint& k) // copy constructor 
    {
        value = new int64_t[k.width];
        width = k.width;
        memcpy(value, k.value, width * sizeof value[0]);
    }

    ~bint ()
    {
        delete[] value;
    }

    void print () const
    {
        int i;

        std::cout << "Elements: " << std::endl;
        for (i = 0; i < width - 1 ; i ++)
        {
            std::cout << value[i] << ", ";
        }
        std::cout << value[i] << std::endl;

        std::cout << "As number: " << std::endl;
        for (i = width - 1; i >= 0; i--)
        {
            std::cout << value[i];
        }
        std::cout << std::endl;
        std::cout << "Width: " << width << std::endl;
    }

    void grow ()
    {
        int32_t newWidth = width * 2;   
        int64_t *newValue = new int64_t[newWidth];
        bzero(newValue, newWidth * sizeof newValue[0]);
        std::memcpy(newValue, value, width * sizeof newValue[0]);
        delete[] value;
        value = newValue;
        width = newWidth;
    }

    void resize (int32_t newWidth)
    {
        int64_t *newValue = new int64_t[newWidth];
        bzero(newValue, newWidth * sizeof newValue[0]);
        std::memcpy(newValue, value, width * sizeof newValue[0]);
        delete[] value;
        value = newValue;
        width = newWidth;
    }

    bint& swap(bint& a)
    {
        int64_t* valueTemp = value;
        int32_t  widthTemp = width;
        value = a.value;
        width = a.width;
        a.value = valueTemp;
        a.width = widthTemp;
        return a;
    }

    bint& sum (const bint& n)
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
        int64_t sum = 0;
        int64_t carry = 0;
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

            if (sum > 9)
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

    bint& sub (const bint& a)
    {
        bint *result = new bint();
        int64_t borrow = 0;

        for (int i = 0; i < WIDTH; i ++)
        {
            int64_t sum = this->value[i] - a.value[i] - borrow;
            if (sum < 0)
            {
                sum = sum + LIMIT + 1;
                borrow = 1;
            }
            else
            {
                borrow = 0;
            }
            result->value[i] = sum;
        }
        // If borrow is set here we have an error
       return *result; 
    }

    bint& mul (const bint a, const bint b)
    {
        bint *result = new bint();

        result->value[0] = this->value[0] + a.value[0];

        return *result; 
    }

private:
    int64_t* value;
    int32_t  width;
};
