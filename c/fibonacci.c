/*
 *  fibonacci.c -- Compute the nth Fibonacci Number
 *  Written December 1, 2018 by Eric Olson
 *
 *  This program demonstrates the expressiveness of C as measured
 *  by explicitly coding the Karatsuba multiplication algorithm for
 *  big-number arithmetic and then using the doubling formulas
 *
 *      F(2k) = F(k)[2F(k+1)-F(k)]
 *      F(2k+1) = F(k+1)^2+F(k)^2
 *      F(2k+2) = F(k+1)[2F(k)+F(k+1)]
 *
 *  to compute the nth Fibonacci number.
 *
 *  Version 2:  Minor changes to fix compiler warnings and zero the
 *  unused memory in the copybn routine.
 *
 *  Version 3:  Jeremy Hall - December 15, 2018
 *    Simplified the 0 and 1 case at the start of fibo() (removed atobn and atolln).
 *    Changed subbn2 to single pass without allocating an array for carries.
 *    Replaced some of the simple logic with max() and min() expressions.
 *    Simplified the print function.
 *    Declared immutable items as const.
 *    Removed multiple call sequences of delbn() (earler calls were ignored).
 *    Used exact width integer types.
 *    More minor changes to fix compiler warnings.
 *    Removed extra jmp from carry settings in Karatsuba multiply.
 *    Changed some function names for slightly better readability.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

#define N 4784969

typedef uint64_t qword;

#define base 1000000000ULL

#define ALLOC_DEPTH 11

#define PHI (1+sqrt(5.0))/2

static const int bn_size = (int)(N * log10(PHI) / 9 + 4);

#define min(a,b) (a < b ? a : b)
#define max(a,b) (a > b ? a : b)

typedef struct {
    qword *d;
    int n;
} bignum;

static qword *bp;

static qword *
fmalloc( const int len ) {
    qword * const r = bp;
    bp += len;
    return r;
}

static void
ffree( const bignum a ) {
    bp = a.d;
}

static bignum
trim_bn( bignum x ) {
    if( x.n == 0 )
        return x;
    int i;
    for( i = x.n - 1; i >= 0; --i )
        if( x.d[i] != 0 )
            break;
    x.n = i + 1;
    return x;
}

static bignum
new_bn( const int digits ) {
    bignum x;
    x.d = fmalloc( digits );
    memset( x.d, 0, (size_t)digits * sizeof(qword) );
    x.n = 0;
    return x;
}

static bignum
set_bn( const qword value ) {
    bignum x = new_bn(bn_size);
    x.d[0] = value;
    x.n = 1;
    return x;
}

static void
print_bn( const bignum x ) {
    printf( "%" PRIu64, x.d[x.n - 1] );
    for( int i = x.n - 2; i >= 0; --i )
        printf( "%09" PRIu64, x.d[i] );
    printf("\n");
}

static void
copy_bn( bignum * const a, bignum b ) {
    b = trim_bn(b);
    if( a->n > b.n )
        memset( a->d + b.n, 0, (size_t)(a->n - b.n) * sizeof(qword) );
    memcpy( a->d, b.d, (size_t)(a->n = b.n) * sizeof(qword) );
}

static bignum
add_bn( const bignum a, const bignum b ) {
    const int n = max( a.n, b.n );
    bignum x = new_bn( n + 1 );
    x.n = n + 1;
    qword c = 0;
    for( int i = 0; i < n; ++i ) {
        x.d[i] = c;
        if( i < a.n )
            c += a.d[i];
        if( i < b.n )
            c += b.d[i];
        if( c >= base ) {
            x.d[i] = c - base;
            c = 1;
        } else {
            x.d[i] = c;
            c = 0;
        }
    }
    x.d[n] = c;
    return x;
}

static void
sub_bn( bignum a, const bignum b ) {
    if( b.n == 0 )
        return;
    uint32_t c = 0;
    for( int i = 0; i < b.n; ++i ) {
        const qword sum = b.d[i] + c;
        c = 0;
        if( a.d[i] < sum ) {
            a.d[i] += base;
            c = 1;
        }
        a.d[i] -= sum;
    }
    a.d[b.n] -= c;
}

static bignum
mul_bn( const bignum a, const bignum b ) {
    if( a.n == 0 || b.n == 0 )
        return new_bn(1);
    bignum x = new_bn( a.n + b.n + 1 );
    x.n = a.n + b.n - 1;
    for( int i = 0; i < a.n; ++i ) {
        for( int j = 0; j < b.n; ++j )
            x.d[i+j] += a.d[i] * b.d[j];
        if( (a.n - i) % 50 == 1 )
            for( int k = 0; k <= x.n; ++k )
                if( x.d[k] >= base ) {
                    const qword c = x.d[k] / base;
                    x.d[k] %= base;
                    x.d[k+1] += c;
                }
    }
    if( x.d[x.n] )
        ++x.n;
    return x;
}

static bignum
mul_karatsuba( const bignum a, const bignum b ) {
    if( a.n == 0 || b.n == 0 )
        return new_bn(1);

    if( min( a.n, b.n ) < 49 )
        return mul_bn( a, b );

    int i, k;
    bignum a0, a1, b0, b1;
    bignum z2, z1a, z1b, z1, z0;
    bignum x = new_bn( a.n + b.n + 1 );

    qword c;
    const int n = max( a.n, b.n ) / 2;
    a0.d = a.d;
    a0.n = min( n, a.n );
    b0.d = b.d;
    b0.n = min( n, b.n );
    a1.d = a.d + n;
    a1.n = max( 0, a.n - n );
    b1.d = b.d + n;
    b1.n = max( 0, b.n - n );
    z0 = mul_karatsuba( a0, b0 );
    z2 = mul_karatsuba( a1, b1 );
    z1a = add_bn( a1, a0 );
    z1b = add_bn( b1, b0 );
    z1 = mul_karatsuba( z1a, z1b );
    sub_bn( z1, z0 );
    sub_bn( z1, z2 );
    z1 = trim_bn(z1);
    memcpy( x.d, z0.d, (size_t)(x.n = z0.n) * sizeof(qword) );
    if( z1.n != 0 ) {
        for( i = 0, c = 0; i < z1.n; ++i ) {
            x.d[k = n + i] += z1.d[i] + c;
            c = 0;
            if( x.d[k] >= base ) {
                x.d[k] -= base;
                c = 1;
            }
        }
        for( k = n + i; c != 0; ++k ) {
            x.d[k] += c;
            c = 0;
            if( x.d[k] >= base ) {
                x.d[k] -= base;
                c = 1;
            }
        }
        if( x.n < k )
            x.n = k;
    }
    if( z2.n != 0 ) {
        const int n2 = n * 2;
        for( i = 0, c = 0; i < z2.n; ++i ) {
            x.d[k = n2 + i] += z2.d[i] + c;
            c = 0;
            if( x.d[k] >= base ) {
                x.d[k] -= base;
                c = 1;
            }
        }
        for( k = n2 + i; c != 0; ++k ) {
            x.d[k] += c;
            c = 0;
            if( x.d[k] >= base ) {
                x.d[k] -= base;
                c = 1;
            }
        }
        if( x.n < k )
            x.n = k;
    }
    ffree(z0);
    return x;
}

static void
fibo( const int n, bignum * const a, bignum * const b ) {
    if( n == 0 ) {
        *a = set_bn(0);
        *b = set_bn(1);
        return;
    }
    fibo( n / 2, a, b );
    bignum ta = *a, tb = *b;
    bignum taa = mul_karatsuba( ta, ta );
    bignum tbb = mul_karatsuba( tb, tb );
    bignum taapbb = add_bn( taa, tbb );
    if( n & 1 ) {
            // [a,b] = [a*a+b*b,b*(2*a+b)]
        bignum t2a = add_bn( ta, ta );
        bignum t2apb = add_bn( t2a, tb );
        bignum tbL2apbR = mul_karatsuba( tb, t2apb );
        copy_bn( a, taapbb );
        copy_bn( b, tbL2apbR );
        ffree(t2a);
    } else {
            // [a,b] = [a*(b*2-a),a*a+b*b]
        bignum t2bma = add_bn( tb, tb );
        sub_bn( t2bma, ta );
        bignum taL2bmaR = mul_karatsuba( ta, t2bma );
        copy_bn( a, taL2bmaR );
        copy_bn( b, taapbb );
        ffree(t2bma);
    }
    ffree(taa);
}

int
main( void ) {
    if( (bp = malloc( (size_t)bn_size * ALLOC_DEPTH * sizeof(qword) )) == NULL ) {
        fprintf(stderr,"Out of memory!\n");
        return EXIT_FAILURE;
    }
    if( N < 2 ) {
        printf( "%d\n", N );
        return EXIT_SUCCESS;
    }
    bignum a, b;
    fibo( N - 1, &a, &b );
    print_bn(b);
    return EXIT_SUCCESS;
}