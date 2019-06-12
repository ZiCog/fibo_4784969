//
// An experiment in doing integer arithmetic using GMP with all numbers represented by strings.
//
// By heater.
//

// Number base used for internal calculations by GMP.
#define IS_BASE 32

// Functions is_let, is_let, is_let and is_letmul do large integer arithmetic on integers represented by strings.

void is_write(const char *s);

char* is_let(const char* s);

char* is_add(const char* s1, const char* s2);

char* is_sub(const char* s1, const char* s2);

char* is_mul(const char* s1, const char* s2);

char* is_base(const char *s, int base);

void is_init();

void is_clear();


