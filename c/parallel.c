/*  parallel.c -- Compute the nth Fibonacci Number
    Written January 7, 2019 by Eric Olson

    This program demonstrates the expressiveness of C with OpenMP
    as measured by explicitly coding a parallel version of the 
    Karatsuba multiplication algorithm for big-number arithmetic 
    and then using the doubling formulas

        F(2k) = F(k)[2F(k+1)-F(k)]
      F(2k+1) = F(k+1)[2F(k+1)-F(k)]+(-1)^(k+1)
      F(2k+1) = F(k)[2F(k)+F(k+1)]+(-1)^(k)
      F(2k+2) = F(k+1)[2F(k)+F(k+1)]

    to compute the nth Fibonacci number.  Note that n is specified
    in the first line of the main routine.
*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <alloca.h>
#include <sys/time.h>
#include <sys/resource.h>

#define TUNE1 44
#define TUNE2 1024
#define TUNE3 1048576

#include "parallel.h"

typedef unsigned long long llu;
typedef int limb;
typedef unsigned long long limb2;
typedef struct {
    int n; limb *d;
} bignum;

static int rexp,cmult,digits,nproc;
static limb rbase,rcarry;
static limb2 cadd,cmax;

static void bigprint(bignum x){
    char fmt[10];
    sprintf(fmt,"%%0%dllu",rexp);
    if(!x.n){
        printf("0\n");
        return;
    }
    for(int i=x.n-1;i>=0;i--){
        if(i==x.n-1) printf("%llu",(llu)x.d[i]);
        else printf(fmt,(llu)x.d[i]);
    }
    printf("\n");
}

static bignum bigtrim(bignum x){
    for(int k=x.n-1;k>=0;k--){
        if(x.d[k]!=0){
            x.n=k+1;
            return x;
        }
    }
    x.n=0;
    return x;
}

static bignum bigcarry(bignum x){
    int imax=x.n-1;
    for(int i=0;i<imax;i++){
        if(x.d[i]>=rbase){
            do {
                x.d[i]-=rbase; x.d[i+1]++;
            } while(x.d[i]>=rbase);
        } else if(x.d[i]<0){
            do {
                x.d[i]+=rbase; x.d[i+1]--;
            } while(x.d[i]<0);
        }
    }
    if(x.d[imax]<rbase) return bigtrim(x);
    x.n++;
    x.d[imax]-=rbase; x.d[imax+1]=1; 
    while(x.d[imax]>=rbase){
        x.d[imax]-=rbase; x.d[imax+1]++;
    }
    return x;
}

static bignum bigcopy(bignum z,bignum x){
    memcpy(z.d,x.d,sizeof(limb)*x.n);
    z.n=x.n;
    return z;
}

static bignum bigadd3(limb zd[],bignum x,bignum y){ 
    bignum z; z.d=zd;
    if(x.n<y.n){
        for(int i=0;i<x.n;i++) z.d[i]=x.d[i]+y.d[i];
        for(int i=x.n;i<y.n;i++) z.d[i]=y.d[i];
        z.n=y.n;
    } else {
        for(int i=0;i<y.n;i++) z.d[i]=x.d[i]+y.d[i];
        for(int i=y.n;i<x.n;i++) z.d[i]=x.d[i];
        z.n=x.n;
    }
    return z;
}

#ifdef NOTUSED
static bignum bigsub3(limb zd[],bignum x,bignum y){ 
    bignum z; z.d=zd;
    for(int i=0;i<y.n;i++) z.d[i]=x.d[i]-y.d[i];
    for(int i=y.n;i<x.n;i++) z.d[i]=x.d[i];
    z.n=x.n;
    return z;
}
#endif

static bignum bigsub2(bignum x,bignum y){ 
    for(int i=0;i<y.n;i++) x.d[i]-=y.d[i];
    return x;
}

static bignum bigadd2(bignum x,bignum y){
    if(x.n<y.n){
        if(x.n>0) {
            x.d[0]--;
            for(int i=0;i<x.n;i++) x.d[i]+=y.d[i]-rcarry;
            x.d[x.n]=y.d[x.n]+1;
            for(int i=x.n+1;i<y.n;i++) x.d[i]=y.d[i];
            x.n=y.n;
        } else {
            x=bigcopy(x,y);
        }
    } else {
        x.d[0]--;
        for(int i=0;i<y.n;i++) x.d[i]+=y.d[i]-rcarry;
        if(y.n==x.n) {
            x.n++;
            x.d[y.n]=1;
        } else {
            x.d[y.n]++;
        }
    }
    return x;
}

static bignum bigdec1(bignum x){ 
    int imax=x.n-1;
    x.d[0]--;
    for(int i=0;i<imax;i++){
        if(x.d[i]>=0) return x;
        x.d[i]+=rbase; x.d[i+1]--;
    }
    return x;
}

static bignum biginc1(bignum x){
    int imax=x.n-1;
    if(imax>=0) {
        x.d[0]++;
        for(int i=0;i<imax;i++){
            if(x.d[i]<rbase) return x;
            x.d[i]-=rbase; x.d[i+1]++;
        }
        if(x.d[imax]<rbase) return x;
        x.d[imax]-=rbase;
    }
    x.n++;
    x.d[imax+1]=1;
    return x;
}

static bignum bigmul3w(limb zd[],bignum x,bignum y){
    bignum z; z.d=zd;
    int imax=x.n+y.n;
    limb2 s[imax];
    bzero(s,sizeof(limb2)*imax);
    imax--;
    for(int i=0;i<x.n;i++){
        for(int j=0;j<y.n;j++){
            s[i+j]+=(limb2)x.d[i]*y.d[j];
        }
        if((i+1)%cmult==0){
            for(int k=0;k<imax;k++){
                if(s[k]<cmax) continue;
                s[k]-=cmax; s[k+1]+=cadd;
            }
        }
    }
    for(int k=0;k<imax;k++){
        s[k+1]+=s[k]/rbase; z.d[k]=s[k]%rbase;
    }
    z.d[imax]=s[imax];
    z.n=imax+1;
    return bigtrim(z);
}

static bignum biglow(bignum x,int n){
    if(x.n>n) x.n=n;
    return x;
}

static bignum bighigh(bignum x,int n){
    if(x.n<n) x.n=0;
    else {
        x.n-=n;
        x.d=&x.d[n];
    }
    return x;
}

static bignum bigmul3s(limb zd[],bignum x,bignum y){
    if(x.n<TUNE1||y.n<TUNE1) return bigmul3w(zd,x,y);
    int M=x.n<y.n?y.n:x.n; int n=M/2;
    bignum z; z.d=zd;
    bignum x0=biglow(x,n),x1=bighigh(x,n);
    bignum y0=biglow(y,n),y1=bighigh(y,n);
    limb z0d[M+1],z1d[M+3],z1yd[n+1],z2d[M+3];
    bignum z0,z2,z1x,z1y,z1;
    z0=bigmul3s(z0d,x0,y0);
    z2=bigmul3s(z2d,x1,y1);
    z1x=bigcarry(bigadd3(z1d,x0,x1));
    z1y=bigcarry(bigadd3(z1yd,y0,y1));
    z1=bigmul3s(z1x.d,z1x,z1y);
    z1=bigcarry(bigsub2(bigsub2(z1,z0),z2));
    z=bigcopy(z,z0);
    z=bigadd3(&z.d[n],(bignum){z.n-n,&z.d[n]},z1);
    z=bigadd2((bignum){z.n-n,&z.d[n]},z2);
    z.n=z.n+2*n; z.d=zd;
    return bigcarry(z);
}

static bignum bigmul3(limb zd[],bignum x,bignum y){
    if(x.n<TUNE1||y.n<TUNE1) return bigmul3w(zd,x,y);
    int M=x.n<y.n?y.n:x.n; int n=M/2;
    if(M<TUNE2) return bigmul3s(zd,x,y);
    bignum z; z.d=zd;
    bignum x0=biglow(x,n),x1=bighigh(x,n);
    bignum y0=biglow(y,n),y1=bighigh(y,n);
    limb z0d[M+1],z1d[M+3],z1yd[n+1],z2d[M+3];
    bignum z0,z2,z1x,z1y,z1;
    spawn(z0=) bigmul3(z0d,x0,y0);
    spawn(z2=) bigmul3(z2d,x1,y1);
    z1x=bigcarry(bigadd3(z1d,x0,x1));
    z1y=bigcarry(bigadd3(z1yd,y0,y1));
    z1=bigmul3(z1x.d,z1x,z1y);
    sync;
    z1=bigcarry(bigsub2(bigsub2(z1,z0),z2));
    z=bigcopy(z,z0);
    z=bigadd3(&z.d[n],(bignum){z.n-n,&z.d[n]},z1);
    z=bigadd2((bignum){z.n-n,&z.d[n]},z2);
    z.n=z.n+2*n; z.d=zd;
    return bigcarry(z);
}

static bignum t1,a,b;
static void fiboworks(int n){
    if(n==0){
        a.n=0; b.n=1; b.d[0]=1;
        return;
    }
    fiboworks(n/2);
    if(n%2==0){
        // [a,b]=[a*(2*b-a),b*(2*b-a)-(-1)^k]
        t1=bigcarry(bigsub2(bigadd3(t1.d,b,b),a));
        a=bigmul3(a.d,a,t1);
        b=bigmul3(b.d,b,t1);
        if(n%4==0) b=bigdec1(b); else b=biginc1(b);
    } else {
        // [a,b]=[a*(2*a+b)+(-1)^k,b*(2*a+b)]
        t1=bigcarry(bigadd2(bigadd3(t1.d,a,a),b));
        b=bigmul3(b.d,b,t1); 
        a=bigmul3(a.d,a,t1); 
        if(n%4==1) a=biginc1(a); else a=bigdec1(a);
    }
    return;
}

static void fibowork(int n){
    if(n==0){
        a.n=0; b.n=1; b.d[0]=1;
        return;
    }
    if(n<TUNE3) fiboworks(n/2);
    else fibowork(n/2);
    if(n%2==0){
        // [a,b]=[a*(2*b-a),b*(2*b-a)-(-1)^k]
        t1=bigcarry(bigsub2(bigadd3(t1.d,b,b),a));
        spawn(a=) bigmul3(a.d,a,t1);
        b=bigmul3(b.d,b,t1);
        if(n%4==0) b=bigdec1(b); else b=biginc1(b);
        sync;
    } else {
        // [a,b]=[a*(2*a+b)+(-1)^k,b*(2*a+b)]
        t1=bigcarry(bigadd2(bigadd3(t1.d,a,a),b));
        spawn(b=) bigmul3(b.d,b,t1); 
        a=bigmul3(a.d,a,t1); 
        if(n%4==1) a=biginc1(a); else a=bigdec1(a);
        sync;
    }
    return;
}

static bignum fibo(int n, limb xd[]){
    b.d=xd;
    if(n<2){
        b.n=1; b.d[0]=n;
        return b;
    }
    limb ad[digits]; a.d=ad;
    fibowork((n-1)/2);
    if(n%2==0){
        // b=b*(2*a+b)
        t1=bigcarry(bigadd2(bigadd3(t1.d,a,a),b));
        b=bigmul3(b.d,b,t1);
    } else {
        // b=b*(2*b-a)-(-1)^k
        t1=bigcarry(bigsub2(bigadd3(t1.d,b,b),a));
        b=bigmul3(b.d,b,t1);
        if(n%4==1) b=bigdec1(b); else b=biginc1(b);
    }
    return b;
}

static int work(int n){
    digits=n*log10(0.5*(1+sqrt(5.0)))/rexp+4;
    limb t1d[digits]; t1.d=t1d;
    limb xd[digits];
    workers(nproc){
        bignum x=fibo(n,xd);
        bigprint(x);
    }
    return 0;
}

int main(int argc, char* argv[]){
    int n=4784969;
    setrlimit(RLIMIT_STACK,
        &(const struct rlimit)
        {RLIM_INFINITY,RLIM_INFINITY});
    if(sizeof(limb)*2!=sizeof(limb2)){
        fprintf(stderr,
        "sizeof(limb)=%llu must be half of sizeof(limb2)=%llu!\n",
        (llu)sizeof(limb),(llu)sizeof(limb2));
        return 1;
    }
    limb limbmax=((limb2)1<<(8*sizeof(limb)-1))-1;
    limb2 limb2max=((limb2)1<<(8*sizeof(limb)))-1;
    limb2max=(limb2max<<(8*sizeof(limb)))+limb2max;
    rexp=log(limbmax)/log(10);
    rbase=pow(10,rexp)+0.5; rcarry=rbase-1;
    cmult=(limb2max-2*rbase)/rbase/rbase/2;
    cadd=(limb2)rbase*cmult; cmax=cadd*rbase;
    nproc=get_ncpu();
    if (argc >= 2) {
        nproc = atol(argv[1]);
    }
    fprintf(stderr, "Using %d cores.\n", nproc);
    return work(n);
}
