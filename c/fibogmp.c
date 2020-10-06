/*  fibogmp.c -- Compute the nth Fibonacci Number
    Written December 24, 2018 by Eric Olson

    This program demonstrates that naively using the doubling formulas

        F(2k) = F(k)[2F(k+1)-F(k)]
      F(2k+1) = F(k+1)[2F(k+1)-F(k)]+(-1)^(k+1)

    and

      F(2k+1) = F(k)[2F(k)+F(k+1)]+(-1)^(k)
      F(2k+2) = F(k+1)[2F(k)+F(k+1)]

    results in 80 percent of the performance of the built-in Fibonacci
    subroutines of the GNU multi-precision GMP library.

    The results of this code can also serve as a point of comparison
    to implementations of the doubling formulas using other languages
    that leverage GMP big numbers behind the scenes.  */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>
#include <sys/time.h>

#ifdef CLOCK_MONOTONIC_RAW
static struct timespec tic_start;
void tic() {
    clock_gettime(CLOCK_MONOTONIC_RAW,&tic_start);
}
double toc() {
    struct timespec tic_stop;
    clock_gettime(CLOCK_MONOTONIC_RAW,&tic_stop);
    double sec=tic_stop.tv_sec-tic_start.tv_sec;
    return sec+(tic_stop.tv_nsec-tic_start.tv_nsec)*1.0e-9;
}
#else
static struct timeval tic_start;
void tic() {
    gettimeofday(&tic_start,0);
}
double toc() {
    struct timeval tic_stop;
    gettimeofday(&tic_stop,0);
    double sec=tic_stop.tv_sec-tic_start.tv_sec;
    return sec+(tic_stop.tv_usec-tic_start.tv_usec)*1.0e-6;
}
#endif

static mpz_t t1,t2,t3;
void fibowork(int n,mpz_t a,mpz_t b){
    if(n==0){
        mpz_set_ui(a,0);
        mpz_set_ui(b,1);
        return;
    }
    fibowork(n/2,a,b);
    if(n%2==0){
        // [a,b]=[a*(2*b-a),b*(2*b-a)-(-1)^k]
        mpz_add(t1,b,b); mpz_sub(t2,t1,a);
        mpz_mul(t1,a,t2); mpz_mul(t3,b,t2);
        if(n%4==0) mpz_sub_ui(b,t3,1);
        else mpz_add_ui(b,t3,1);
        mpz_set(a,t1);
    } else {
        // [a,b]=[a*(2*a+b)+(-1)^k,b*(2*a+b)]
        mpz_add(t1,a,a); mpz_add(t2,t1,b);
        mpz_mul(t1,b,t2); mpz_mul(t3,a,t2);
        if(n%4==1) mpz_add_ui(a,t3,1);
        else mpz_sub_ui(a,t3,1);
        mpz_set(b,t1);
    }
    return;
}

static mpz_t a;
void fibo(int n,mpz_t b){
    if(n<2){
        mpz_set_ui(b,n);
        return;
    }
    fibowork((n-1)/2,a,b);
    if(n%2==0){
        // b=b*(2*a+b)
        mpz_add(t1,a,a); mpz_add(t2,t1,b); mpz_mul(t1,b,t2);
        mpz_set(b,t1);
    } else {
        // b=b*(2*b-a)-(-1)^k
        mpz_add(t1,b,b); mpz_sub(t2,t1,a); mpz_mul(t3,b,t2);
        if(n%4==1) mpz_sub_ui(b,t3,1);
        else mpz_add_ui(b,t3,1);
    }
    return;
}

#define bits 4000000
int main(){
    mpz_t c,b;
    int n=4784969;
    mpz_init2(a,bits); mpz_init2(b,bits); mpz_init2(c,bits);
    mpz_init2(t1,bits); mpz_init2(t2,bits); mpz_init2(t3,bits);

    double t,tg=1e7,tgtot=1e7,tf=1e7,tftot=1e7;

    for(int i=0;i<3;i++){
        tic();
        mpz_fib_ui(c,n);
        t=toc();
        if(t<tg) tg=t;
        mpz_out_str(stdout,10,c);
        putchar('\n');
        t=toc();
        if(t<tgtot) tgtot=t;
        fflush(stdout);

        tic();
        fibo(n,b);
        t=toc();
        if(t<tf) tf=t;
        mpz_out_str(stdout,10,b);
        putchar('\n');
        t=toc();
        if(t<tftot) tftot=t;
        fflush(stdout);
    }

    if(mpz_cmp(c,b)){
        fprintf(stderr,"Failed consistency check!\n");
    }
    fprintf(stderr,"%12s %12s %12s %12s\n",
        "","GMP","Doubling","Percent");
    fprintf(stderr,"%12s %12.9f %12.9f %12.5f\n",
        "fibo",tg,tf,tg/tf*100);
    fprintf(stderr,"%12s %12.9f %12.9f %12.5f\n","print",
        tgtot-tg,tftot-tf,(tgtot-tg)/(tftot-tf)*100);
    fprintf(stderr,"%12s %12.9f %12.9f %12.5f\n",
        "total",tgtot,tftot,tgtot/tftot*100);
    return 0;
}