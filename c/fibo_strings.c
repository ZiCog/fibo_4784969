//
// An experiment in doing integer arithmetic using GMP with all numbers represented by strings.
//
// By heater.
// Modified June 11, 2019 to use base 32 strings for intermediate results.
//
#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

// Number base used for internal calculations by GMP.
int BASE = 32;

mpz_t op1;
mpz_t op2;
mpz_t res;

// Functions letis, addis, subis and mulis do large integer arithmetic on integers represented by strings.

void writeis(const char *s) {
    mpz_set_str (op1, s, BASE);
    char *buf=mpz_get_str (0, 10, op1);
    puts(buf);
    free(buf);
}

char* letis(const char* s) {
    return strdup(s);
}

char* addis(const char* s1, const char* s2) {
    mpz_set_str (op1, s1, BASE);
    mpz_set_str (op2, s2, BASE);
    mpz_add (res, op1, op2);  // result = x * y
    char* res_string = mpz_get_str (0, BASE, res); 
    return res_string;
}

char* subis(const char* s1, const char* s2) {
    mpz_set_str (op1, s1, BASE);
    mpz_set_str (op2, s2, BASE);
    mpz_sub (res, op1, op2);  // result = x * y
    char* res_string = mpz_get_str (0, BASE, res); 
    return res_string;
}

char* mulis(const char* s1, const char* s2) {
    mpz_set_str (op1, s1, BASE);
    mpz_set_str (op2, s2, BASE);
    mpz_mul (res, op1, op2);  // result = x * y
    char* res_string = mpz_get_str (0, BASE, res); 
    return res_string;
}

char* memo[3];

void init_memo() {
    memo[0] = letis("0");
    memo[1] = letis("1");
    memo[2] = letis("1");
}

void clean_memo() {
    free(memo[0]);
    free(memo[1]);
    free(memo[2]);
}


// Return the n'th Fibonacci number as a decimal string for integer n
char* fibois (int n) {
    char* res;
    if (n <= 2) {
        return letis(memo[n]);
    }

    int k = (n / 2);
    char* fk = fibois(k);
    char* fk1 = fibois(k + 1);
    char* a;
    char* b;
    if ((n % 2) == 0) {
        a = addis(fk1, fk1);
        b = subis(a, fk);
        res = mulis(fk, b);
    } else {
        a = mulis(fk, fk);
        b = mulis(fk1, fk1);
        res = addis(a, b);
    }
    free(a);
    free(b);
    free(fk);
    free(fk1);
    return res;
}

int main(int argc, char* argv[]) {
    char* f;
    int n = 4784969;               // The first Fibonacci number with a million digits

    if (argc >= 2) {
        n = atoi(argv[1]);
    }

    mpz_init(op1);
    mpz_init(op2);
    mpz_init(res);

    init_memo();

    f = fibois(n);
    writeis(f);
    free(f);

    clean_memo();

    mpz_clear(op1);
    mpz_clear(op2);
    mpz_clear(res);

    return (0);
}
