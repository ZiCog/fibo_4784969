/*  fibonacci.c -- Compute the nth Fibonacci Number
    Written December 1, 2018 by Eric Olson

    This program demonstrates the expressiveness of C as measured
    by explicitly coding the Karatsuba multiplication algorithm for
    big-number arithmetic and then using the doubling formulas

        F(2k) = F(k)[2F(k+1)-F(k)]
      F(2k+1) = F(k+1)^2+F(k)^2
      F(2k+2) = F(k+1)[2F(k)+F(k+1)]

    to compute the nth Fibonacci number.  Note that n is specified
    in the first line of the main routine.

    Version 2:  Minor changes to fix compiler warnings and zero the
    unused memory in the copybn routine.
*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned long long ull;
static const ull base=1000000000LL;
static const int bexp=9;
 
static unsigned char *bnbuf,*bp;

static inline void *fmalloc(unsigned int p){
    unsigned char *r=bp; bp+=p; return r;
}
static inline void ffree(void *p){
    bp=p;
}
 
typedef struct {
    int n;
    ull *d;
} bignum;
 
static ull atolln(char *p,int n){
    int i;
    ull e,s=0;
    for(i=n-1,e=1;i>=0;i--,e*=10){
        s+=(p[i]-'0')*e;
    }
    return s;
}

static void pbn(bignum x){
    int i;
    char fmt[8];
    sprintf(fmt,"%%0%dLu",bexp);
    if(!x.n){
        printf("0\n");
        return;
    }
    for(i=x.n-1;i>=0;i--){
        if(i==x.n-1) printf("%Lu",x.d[i]);
        else printf(fmt,x.d[i]);
    }
    printf("\n");
}
 
static inline bignum trmbn(bignum x){
    int i;
    if(!x.n) return x;
    for(i=x.n-1;i>=0;i--) if(x.d[i]) break;
    x.n=i+1;
    return x;
}
 
static inline bignum newbn(int digits){
    bignum x;
    x.d=fmalloc(digits*sizeof(ull));
    memset(x.d,0,digits*sizeof(ull));
    x.n=0;
    return x;
}
 
static bignum atobn(char *p,int digits){
    int i;
    bignum x=newbn(digits);
    int n=strlen(p);
    if(!n) return x;
    for(i=n-bexp;i>=0;i-=bexp){
        x.d[x.n++]=atolln(&p[i],bexp);
    }
    if(i+bexp>0) {
        x.d[x.n++]=atolln(&p[0],i+bexp);
    }
    return x;
}
 
static void inline delbn(bignum a){
    ffree(a.d);
}

static bignum addbn(bignum a,bignum b){
    ull c;
    bignum x;
    int i,n;
    n=a.n>b.n?a.n:b.n;
    x=newbn(n+1);
    for(i=0,c=0;i<n;i++){
        x.d[i]=c;
        if(i<a.n) c+=a.d[i];
        if(i<b.n) c+=b.d[i];
        if(c>=base) {
            x.d[i]=c-base;
            c=1;
        } else {
            x.d[i]=c;
            c=0;
        }
    }
    if(c) x.d[n++]=c;
    x.n=n;
    return x;
}
 
static void subbn2(bignum a,bignum b){
    int i;
    char *c;
    if(!b.n) return;
    c=fmalloc(a.n+1);
    for(i=0,c[0]=0;i<b.n;i++){
        if(a.d[i]<b.d[i]+c[i]) {
            a.d[i]+=base;
            c[i+1]=1;
        } else {
            c[i+1]=0;
        }
    }
    for(i=0;i<b.n;i++){
        a.d[i]-=b.d[i]+c[i];
    }
    a.d[b.n]-=c[b.n];
    ffree(c);
    return;
}
 
static bignum mulbn(bignum a,bignum b){
    bignum x;
    int i,k,j;
    ull c;
    if(!a.n||!b.n) {
        x=newbn(1);
        return x;
    }
    x=newbn(a.n+b.n+1);
    x.n=a.n+b.n-1;
    for(i=0;i<a.n;i++) {
        for(j=0;j<b.n;j++){
            x.d[i+j]+=a.d[i]*b.d[j];
        }
        if((a.n-i)%50==1){
            for(k=0;k<=x.n;k++){
                if(x.d[k]>=base){
                    c=x.d[k]/base;
                    x.d[k]%=base;
                    x.d[k+1]+=c;
                }
            }
        }
    }
    if(x.d[x.n]) x.n++;
    return x;
}
 
static bignum mulbnk(bignum a, bignum b){
    bignum x;
    int M,m,i,n;
    ull c;
    bignum a0,a1,b0,b1;
    bignum z2,z1a,z1b,z1,z0;
    if(!a.n||!b.n) {
        x=newbn(1);
        return x;
    }
    x=newbn(a.n+b.n+1);
    if(a.n>b.n) { M=a.n; m=b.n; }
    else { M=b.n; m=a.n; }
    if(m<49) return mulbn(a,b);
    n=M/2;
    a0.d=&a.d[0]; a0.n=n; if(a.n<a0.n) a0.n=a.n;
    b0.d=&b.d[0]; b0.n=n; if(b.n<b0.n) b0.n=b.n;
    a1.d=&a.d[n]; a1.n=a.n-n; if(a1.n<0) a1.n=0;
    b1.d=&b.d[n]; b1.n=b.n-n; if(b1.n<0) b1.n=0;
    z0=mulbnk(a0,b0);
    z2=mulbnk(a1,b1);
    z1a=addbn(a1,a0);
    z1b=addbn(b1,b0);
    z1=mulbnk(z1a,z1b);
    subbn2(z1,z0);
    subbn2(z1,z2);
    z1=trmbn(z1);
    memcpy(x.d,z0.d,(x.n=z0.n)*sizeof(ull));
    if(z1.n){
        int k;
        for(i=0,c=0;i<z1.n;i++){
            x.d[k=n+i]+=z1.d[i]+c;
            if(x.d[k]>=base) {
                x.d[k]-=base;
                c=1;
            } else c=0;
        }
        k=n+i;
        while(c){
            x.d[k]+=c;
            if(x.d[k]>=base) {
                x.d[k]-=base;
                c=1;
            } else c=0;
            k++;
        }
        if(x.n<k) x.n=k;
    }
    if(z2.n){
        int n2=2*n,k;
        for(i=0,c=0;i<z2.n;i++){
            x.d[k=n2+i]+=z2.d[i]+c;
            if(x.d[k]>=base) {
                x.d[k]-=base;
                c=1;
            } else c=0;
        }
        k=n2+i;
        while(c){
            x.d[k]+=c;
            if(x.d[k]>=base) {
                x.d[k]-=base;
                c=1;
            } else c=0;
            k++;
        }
        if(x.n<k) x.n=k;
    }
    delbn(z1); delbn(z1b); delbn(z1a); delbn(z2); delbn(z0);
    return x;
}
 
static void copybn(bignum *a,bignum b){
    b=trmbn(b);
    if(a->n>b.n) bzero(a->d+b.n,sizeof(ull)*(a->n-b.n));
    memcpy(a->d,b.d,sizeof(ull)*(a->n=b.n));
}

static int digits;    
static int fibo(int n,bignum *a,bignum *b){
    if(!n) {
        *a=atobn("0",digits);
        *b=atobn("1",digits);
        return 0;
    }
    int m=2*fibo(n>>1,a,b)+n%2;
    bignum ta=*a, tb=*b;
    bignum taa=mulbnk(ta,ta);
    bignum tbb=mulbnk(tb,tb);
    bignum taapbb=addbn(taa,tbb);
    if(n%2){
        // [a,b]=[a*a+b*b,b*(2*a+b)]
        bignum t2a=addbn(ta,ta);
        bignum t2apb=addbn(t2a,tb);
        bignum tbL2apbR=mulbnk(tb,t2apb);
        copybn(a,taapbb); copybn(b,tbL2apbR);
        delbn(tbL2apbR); delbn(t2apb); delbn(t2a);
    } else {
        // [a,b]=[a*(b*2-a),a*a+b*b]
        bignum t2bma=addbn(tb,tb); subbn2(t2bma,ta);
        bignum taL2bmaR=mulbnk(ta,t2bma);
        copybn(a,taL2bmaR); copybn(b,taapbb);
        delbn(taL2bmaR); delbn(t2bma);
    }
    delbn(taapbb); delbn(tbb); delbn(taa);
    return m;
}

#define PHI ((1+sqrt(5.0))/2)
#define MFACT 12

int main(){
    int n=4784969;
    digits=n*log10(PHI)/bexp+4;
    bnbuf=malloc(digits*MFACT*sizeof(ull));
    if(!bnbuf){
        fprintf(stderr,"Out of memory!\n");
        return 1;
    }
    bp=bnbuf;
    bignum a,b;
    if(n<2) printf("%d\n",n);
    else {
        fibo(n-1,&a,&b);
        pbn(b);
    }
    return 0;
}
