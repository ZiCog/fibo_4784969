rem fibo.bas -- Compute the nth Fibonacci Number
rem Written December 6, 2018 by Eric Olson
rem
rem This program demonstrates the expressiveness of FreeBASIC as 
rem measured by explicitly coding big-number arithmetic and then
rem using the doubling formulas
rem
rem     F(2k) = F(k)[2F(k+1)-F(k)]
rem   F(2k+1) = F(k+1)^2+F(k)^2
rem   F(2k+2) = F(k+1)[2F(k)+F(k+1)]
rem
rem to compute the nth Fibonacci number.  For simplicity this code
rem does not use Karatsuba multiplication but instead relies on
rem the O(n^2) algorithm in all cases.  When computing F(4784969)
rem this resutls in significantly more computational work.
rem
rem Version 2:  Optimizations in the O(n^2) multiplication routine
rem to reduce the number of divisions and refactoring the code for
rem the doubling to avoid computing a on the last return.
rem
rem Version 3:  Optimized how the doubling formulas are written as
rem
rem     F(2k) = F(k)[2F(k+1)-F(k)]
rem   F(2k+1) = F(k+1)[2F(k+1)-F(k)]+(-1)^(k+1)
rem   F(2k+1) = F(k)[2F(k)+F(k+1)]+(-1)^(k)
rem   F(2k+2) = F(k+1)[2F(k)+F(k+1)]

const as integer nmax = 250100
const as uinteger radix = 10000
const as integer rexp = 4

type bignum
    dim as integer n
    dim as uinteger d(1 to nmax)
end type

sub bigprint(x as bignum)
    dim as integer i
    if x.n=0 then
        print 0
    else
        for i=x.n to 1 step -1
            dim as string s=str(x.d(i))
            if i<x.n then
                s=mid("0000000000",1,rexp-len(s))+s
            end if
            print s;
        next i
        print
    endif
end sub

sub bigmul(z as bignum,x as bignum,y as bignum)
    dim as integer i,j,k
    for i=1 to x.n+y.n
        z.d(i)=0
    next i
    for i=1 to x.n
        for j=1 to y.n
            z.d(i+j-1)=z.d(i+j-1)+x.d(i)*y.d(j)
        next j
        if (x.n-i) mod 40 = 0 then
            for k=1 to x.n+y.n-1
                if z.d(k)>=radix then
                    dim as uinteger c
                    c=z.d(k)\radix
                    z.d(k)=z.d(k) mod radix
                    z.d(k+1)=z.d(k+1)+c
                end if
            next k
        end if
    next i
    z.n=x.n+y.n
    while z.d(z.n)=0 and z.n>1
        z.n=z.n-1
    wend
    return
end sub

sub biginc(z as bignum)
    dim as integer i=1
    dim as uinteger c=1
    while c
        dim as uinteger t=z.d(i)+1
        if t>=radix then
            z.d(i)=t-radix
        else
            z.d(i)=t
            c=0
        end if
        i=i+1
    wend
end sub

sub bigadd(z as bignum,x as bignum,y as bignum)
    if x.n<y.n then 
        dim as integer i
        dim as uinteger c=0
        for i=1 to x.n
            dim as uinteger t=x.d(i)+y.d(i)+c
            if t>=radix then
                z.d(i)=t-radix
                c=1
            else
                z.d(i)=t
                c=0
            end if
        next i
        i=x.n+1
        z.n=y.n+1
        y.d(z.n)=0
        while c
            dim as uinteger t=y.d(i)+1
            if t>=radix then
                z.d(i)=t-radix
            else
                z.d(i)=t
                c=0
            end if
            i=i+1
        wend
        while i<=z.n
            z.d(i)=y.d(i)
            i=i+1
        wend
    else
        dim as integer i
        dim as uinteger c=0
        for i=1 to y.n
            dim as uinteger t=x.d(i)+y.d(i)+c
            if t>=radix then
                z.d(i)=t-radix
                c=1
            else
                z.d(i)=t
                c=0
            end if
        next i
        i=y.n+1
        z.n=x.n+1
        x.d(z.n)=0
        while c
            dim as uinteger t=x.d(i)+1
            if t>=radix then
                z.d(i)=t-radix
            else
                z.d(i)=t
                c=0
            end if
            i=i+1
        wend
        while i<=z.n
            z.d(i)=x.d(i)
            i=i+1
        wend
    end if
    while z.d(z.n)=0 and z.n>1
        z.n=z.n-1
    wend
    return
end sub

sub bigdec(z as bignum)
    dim as integer i=1
    dim as uinteger c=1
    while c
        if z.d(i)<1 then
            z.d(i)=radix-1
        else
            z.d(i)=z.d(i)-1
            c=0
        end if
        i=i+1
    wend
end sub

sub bigsub(z as bignum,x as bignum,y as bignum)
    dim as integer i
    dim as uinteger c=0
    for i=1 to y.n
        dim as uinteger t=y.d(i)+c
        if x.d(i)<t then
            z.d(i)=x.d(i)+radix-t
            c=1
        else
            z.d(i)=x.d(i)-t
            c=0
        end if
    next i
    i=y.n+1
    while c
        if x.d(i)<1 then
            z.d(i)=x.d(i)+radix-1
        else
            z.d(i)=x.d(i)-1
            c=0
        end if
        i=i+1
    wend
    while i<=x.n
        z.d(i)=x.d(i)
        i=i+1
    wend
    z.n=x.n
    while z.d(z.n)=0 and z.n>1
        z.n=z.n-1
    wend
    return
end sub

dim shared as bignum t1,t2,t3
sub fibowork(n as integer,a as bignum,b as bignum)
    if n=0 then
        a.n=0:b.n=1:b.d(1)=1
        return
    end if
    fibowork(n\2,a,b)
    if n mod 2=0 then
        rem [a,b]=[a*(2*b-a),b*(2*b-a)-(-1)^k]
        bigadd(t1,b,b):bigsub(t2,t1,a)
        bigmul(t1,a,t2):bigmul(t3,b,t2)
        if n mod 4=0 then bigdec(t3) else biginc(t3)
        a=t1:b=t3
    else
        rem [a,b]=[a*(2*a+b)+(-1)^k,b*(2*a+b)]
        bigadd(t1,a,a):bigadd(t2,t1,b)
        bigmul(t1,b,t2):bigmul(t3,a,t2)
        if n mod 4=1 then biginc(t3) else bigdec(t3)
        a=t3:b=t1
    end if
    return
end sub

sub fibo(n as integer,b as bignum)
    if n<2 then
        b.n=1:b.d(1)=n
        return
    end if
    static as bignum a
    fibowork((n-1)\2,a,b)
    if n mod 2=0 then
        rem b=b*(2*a+b)
        bigadd(t1,a,a):bigadd(t2,t1,b):bigmul(t1,b,t2)
        b=t1
    else
        rem b=b*(2*b-a)-(-1)^k
        bigadd(t1,b,b):bigsub(t2,t1,a):bigmul(t3,b,t2)
        if n mod 4=1 then bigdec(t3) else biginc(t3)
        b=t3
    end if
    return
end sub

rem main routine
dim as integer n=4784969
static as bignum b
fibo(n,b)
bigprint(b)
