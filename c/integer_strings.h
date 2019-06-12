//
// An experiment in doing integer arithmetic using GMP with all numbers represented by strings.
//
// By heater.
//

// Number base used for internal calculations by GMP.
#define IS_BASE 32

// Functions letis, addis, subis and mulis do large integer arithmetic on integers represented by strings.

void is_write(const char *s);

char* is_let(const char* s);

char* is_add(const char* s1, const char* s2);

char* is_sub(const char* s1, const char* s2);

char* is_mul(const char* s1, const char* s2);

char* is_base10(const char *s);

void is_init();

void is_clear();


