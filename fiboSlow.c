#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#define N 100000

// 18 9's
//#define LIM  999999999999999999
//#define LIM  99999999999
#define LIM 100000000000

uint64_t a[N];
uint64_t b[N];
uint64_t c[N];

int count = 0;

void puti (uint64_t *a)
{
    char leadingZero = 1;

    for (int i = N - 1; i >= 0; i--)
    {
        if (leadingZero)
        {
            if ((a[i] != 0) || (i == 0))
            {
                printf("%0" PRIu64, a[i]);
                leadingZero = 0;
            }
        }
        else
        {
            printf("%0*" PRIu64, 11, a[i]);
        }
    }    
}

void addi (uint64_t *a, uint64_t *b, uint64_t *c)
{
    uint64_t j;
    uint64_t sum;
    int carry;

    carry = 0;

    for (j = 0; j < N; j++) {
        sum = b[j] + c[j] + carry;
        if (sum > LIM)
        {
            a[j] = sum - LIM;
            carry = 1;
        }
        else {
            if (sum == LIM)
            {
                //printf("Error: Count = %d\n", count);
                //break;
            }
            a[j] = sum;
            carry = 0;
        }
    }
}

int main (int argc, char* argv[])
{
    for (int i = 0; i < N; i++) {
        a[i] = 0;
        b[i] = 0;
        c[i] = 0;
    }

    b[0] = 0;
    c[0] = 1;

    int n = 4784969;  // The first fibo with 1 million digits
    n = 1528930; // Gives error
    n = 1528931;

    while (1)
    {
        addi(a, b, c);
        count++;
        if (count == n)
        {
            //printf("%7d: ", count);
            puti(a);
            printf("\n");
            break;
        }

        addi(c, b, a);
        count++;
        if (count == n)
        {
            //printf("%7d: ", count);
            puti(c);
            printf("\n");
            break;
        }

        addi(b, a, c);
        count++;
        if (count == n)
        {
            //printf("%7d: ", count);
            puti(b);
            printf("\n");
            break;
        }
    }

    return 0;
}