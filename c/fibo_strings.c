//
// An experiment in doing integer arithmetic using GMP with all numbers represented by strings.
//
// By heater.
//
#include <stdio.h>
#include <stdlib.h>

#include "integer_strings.h"

char* fibos[3];

// Return the n'th Fibonacci number as a decimal string for integer n
char* fibo (int n) {
    char* res;
    if (n <= 2) {
        return is_let(fibos[n]);
    }

    int k = (n / 2);
    char* fk = fibo(k);
    char* fk1 = fibo(k + 1);
    char* a;
    char* b;
    if ((n % 2) == 0) {
        a = is_add(fk1, fk1);
        b = is_sub(a, fk);
        res = is_mul(fk, b);
    } else {
        a = is_mul(fk, fk);
        b = is_mul(fk1, fk1);
        res = is_add(a, b);
    }
    free(a);
    free(b);
    free(fk);
    free(fk1);
    return res;
}

int main(int argc, char* argv[]) {
    int n = 4784969;               // The first Fibonacci number with a million digits

    if (argc >= 2) {
        n = atoi(argv[1]);
    }

    is_init();

    fibos[0] = is_let("0");
    fibos[1] = is_let("1");
    fibos[2] = is_let("1");

    char* f = fibo(n);
    char* f10 = is_base(f, 10);
    puts(f10);
    free(f10);
    free(f);

    free(fibos[0]);
    free(fibos[1]);
    free(fibos[2]);

    is_clear();

    return (0);
}
