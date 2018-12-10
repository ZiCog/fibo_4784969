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
rem this results in significantly more computational work.

const as integer nmax = 250100
const as integer radix = 10000
const as integer rexp = 4

type bignum
    dim as integer n
    dim as integer d(1 to nmax)
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
    dim as integer i,j
    for i=1 to x.n+y.n
        z.d(i)=0
    next i
    for i=1 to x.n
        for j=1 to y.n
            dim as integer t,c
            c=0
            t=z.d(i+j-1)+x.d(i)*y.d(j)
            if t>=radix then
                c=t\radix
                t=t mod radix
            end if
            z.d(i+j-1)=t
            z.d(i+j)=z.d(i+j)+c
        next j
    next i
    z.n=x.n+y.n
    while z.d(z.n)=0 and z.n>1
        z.n=z.n-1
    wend
    return
end sub

sub bigadd(z as bignum,x as bignum,y as bignum)
    dim as integer i,max
    if y.n>x.n then
        max=y.n
    else
        max=x.n
    end if
    for i=1 to max+1
        z.d(i)=0
    next i
    for i=1 to max
        dim as integer t,c
        c=0
        if i<=x.n then
            t=z.d(i)+x.d(i)
            if i<=y.n then
                t=t+y.d(i)
            end if
        else
            t=z.d(i)+y.d(i)
        end if
        if t>=radix then
            c=1
            t=t-radix
        end if
        z.d(i)=t
        z.d(i+1)=z.d(i+1)+c
    next i
    z.n=max+1
    while z.d(z.n)=0 and z.n>1
        z.n=z.n-1
    wend
    return
end sub

sub bigsub(z as bignum,x as bignum,y as bignum)
    dim as integer i
    for i=1 to x.n
        z.d(i)=0
    next i
    for i=1 to y.n
        dim as integer t,c
        c=0
        t=z.d(i)+x.d(i)-y.d(i)
        if t<0 then
            t=t+radix
            c=-1
        end if
        z.d(i)=t
        z.d(i+1)=z.d(i+1)+c
    next i
    for i=y.n+1 to x.n
        dim as integer t,c
        c=0
        t=z.d(i)+x.d(i)
        if t<0 then
            t=t+radix
            c=-1
        end if
        z.d(i)=t
        z.d(i+1)=z.d(i+1)+c
    next i
    z.n=x.n
    while z.d(z.n)=0 and z.n>1
        z.n=z.n-1
    wend
    return
end sub

sub fibo(n as integer,a as bignum,b as bignum)
    if n=0 then
        a.n=0:b.n=1:b.d(1)=1
        return
    end if
    fibo(n\2,a,b)
    static as bignum t1,t2,t3
    bigmul(t1,a,a):bigmul(t2,b,b):bigadd(t3,t1,t2)
    if n mod 2=0 then
        rem [a,b]=[a*(b*2-a),a*a+b*b]
        bigadd(t1,b,b):bigsub(t2,t1,a):bigmul(t1,a,t2)
        a=t1:b=t3
    else
        rem [a,b]=[a*a+b*b,b*(2*a+b)]
        bigadd(t1,a,a):bigadd(t2,t1,b):bigmul(t1,b,t2)
        a=t3:b=t1
    end if
    return
end sub

rem main routine
dim as integer n=4784969
if n<2 then
    print str(n)
else
    static as bignum a,b
    fibo(n-1,a,b)
    bigprint(b)
end if