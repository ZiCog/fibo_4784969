//
// An experiment in doing integer arithmetic using GMP with all numbers represented by strings.
//
// By heater.
// Modified June 11, 2019 to use base 32 strings for intermediate results.
//
#include <gmp.h>
#include <string.h>

#include "integer_strings.h"

// Functions letis, addis, subis and mulis do large integer arithmetic on integers represented by strings.
// WARNING: Not thread safe due to use of global op1, op2, res.

static mpz_t op1;
static mpz_t op2;
static mpz_t res;

char* is_base(const char *s, int base) {
    mpz_set_str (op1, s, IS_BASE);
    char* res_string = mpz_get_str (0, base, op1);
    return res_string;
}

char* is_let(const char* s) {
    return strdup(s);
}

char* is_add(const char* s1, const char* s2) {
    mpz_set_str (op1, s1, IS_BASE);
    mpz_set_str (op2, s2, IS_BASE);
    mpz_add (res, op1, op2);  // result = x * y
    char* res_string = mpz_get_str (0, IS_BASE, res); 
    return res_string;
}

char* is_sub(const char* s1, const char* s2) {
    mpz_set_str (op1, s1, IS_BASE);
    mpz_set_str (op2, s2, IS_BASE);
    mpz_sub (res, op1, op2);  // result = x * y
    char* res_string = mpz_get_str (0, IS_BASE, res); 
    return res_string;
}

char* is_mul(const char* s1, const char* s2) {
    mpz_set_str (op1, s1, IS_BASE);
    mpz_set_str (op2, s2, IS_BASE);
    mpz_mul (res, op1, op2);  // result = x * y
    char* res_string = mpz_get_str (0, IS_BASE, res); 
    return res_string;
}

void is_init() {
    mpz_init(op1);
    mpz_init(op2);
    mpz_init(res);

}

void is_clear() {
    mpz_clear(op1);
    mpz_clear(op2);
    mpz_clear(res);
}


