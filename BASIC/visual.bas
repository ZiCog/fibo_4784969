module fibonacci

'    visual.bas -- Compute the nth Fibonacci Number
'    Written January 14, 2018 by Eric Olson
'
'    This program demonstrates the expressiveness of Visual Basic
'    in the mono virtual machine as measured by explicitly coding
'    big-number arithmetic and then using the doubling formulas
'    
'        F(2k) = F(k)[2F(k+1)-F(k)]
'      F(2k+1) = F(k+1)[2F(k+1)-F(k)]+(-1)^(k+1)
'
'    and
'
'      F(2k+1) = F(k)[2F(k)+F(k+1)]+(-1)^(k)
'      F(2k+2) = F(k+1)[2F(k)+F(k+1)]
'     
'    to compute the nth Fibonacci number.

dim cmult,rexp,nmax as integer
dim radix as uinteger
dim cadd,cmax as ulong

sub bigprint(x() as uinteger)
    dim i as integer
    if x(0)=0 then
        console.writeline("0")
    else
        for i=x(0) to 1 step -1
            dim s as string
            dim j as integer
            j=x(i):s=str(j)
            if mid(s,1,1)=" " then
                s=mid(s,2,len(s)-1)
            end if
            if i<x(0) then
                s=mid("0000000000",1,rexp-len(s))+s
            end if
            console.write(s)
            next i
        console.writeline()
    end if
end sub

sub biginc(z() as uinteger)
    dim i as integer
    dim t,c as uinteger
    i=1:c=1
    while c
        t=z(i)+1
        if t>=radix then
            z(i)=t-radix
        else
            z(i)=t
            c=0
        end if
        i=i+1
    end while
end sub

sub bigadd(z() as uinteger,x() as uinteger,y() as uinteger)
    dim i,max as integer
    if y(0)>x(0) then
        max=y(0)
    else
        max=x(0)
    end if
    for i=1 to max+1
        z(i)=0
    next i
    for i=1 to max
        dim t,c as integer
        c=0
        if i<=x(0) then
            t=z(i)+x(i)
            if i<=y(0) then
                t=t+y(i)
            end if
        else
            t=z(i)+y(i)
        end if
        if t>=radix then
            c=1
            t=t-radix
        end if
        z(i)=t
        z(i+1)=z(i+1)+c
    next i
    z(0)=max+1
    while z(z(0))=0 and z(0)>1
        z(0)=z(0)-1
    end while
    return
end sub

sub bigdec(z() as uinteger)
    dim i as integer
    dim c as uinteger
    i=1:c=1
    while c
        if z(i)<1 then
            z(i)=radix-1
        else
            z(i)=z(i)-1
            c=0
        end if
        i=i+1
    end while
end sub

sub bigsub(z() as uinteger,x() as uinteger,y() as uinteger)
    dim i as integer
    dim t,c as uinteger
    c=0
    for i=1 to y(0)
        t=y(i)+c
        if x(i)<t then
            z(i)=x(i)+radix-t
            c=1
        else
            z(i)=x(i)-t
            c=0
        end if
    next i
    i=y(0)+1
    while c
        if x(i)<1 then
            z(i)=x(i)+radix-1
        else
            z(i)=x(i)-1
            c=0
        end if
        i=i+1
    end while
    while i<=x(0)
        z(i)=x(i)
        i=i+1
    end while
    z(0)=x(0)
    while z(z(0))=0 and z(0)>1
        z(0)=z(0)-1
    end while
    return
end sub

dim xy() as ulong
sub bigmulw(z() as uinteger,x() as uinteger,y() as uinteger)
    dim i,j,imax as integer
    imax=x(0)+y(0)
    for i=1 to imax
        xy(i)=0
    next i
    for i=1 to x(0)
        for j=1 to y(0)
            dim xt,yt as ulong
            xt=x(i):yt=y(j)
            xy(i+j-1)=xy(i+j-1)+xt*yt
        next j
        if(i mod cmult = 0) then
            dim k,kmax as integer
            kmax=i+y(0)-1
            for k=1 to kmax
                if(xy(k)>=cmax) then
                    xy(k)=xy(k)-cmax
                    xy(k+1)=xy(k+1)+cadd
                end if
            next k
        end if
    next i
    for i=1 to imax-1
        xy(i+1)=xy(i+1)+xy(i)\radix
        z(i)=xy(i) mod radix
    next i
    z(imax)=xy(imax)
    while z(imax)=0 and imax>1
        imax=imax-1
    end while 
    z(0)=imax
    return
end sub

sub bighigh(z() as uinteger,x() as uinteger,n as integer)
    if x(0) <= n then
        z(0)=0
        return
    end if
    dim i as integer
    for i=n+1 to x(0)
        z(i-n)=x(i)
    next i
    z(0)=x(0)-n
    return
end sub

sub biglow(z() as uinteger,x() as uinteger,n as integer)
    if x(0) <= n then z(0)=x(0) else z(0)=n
    dim i as integer
    for i=1 to z(0)
        z(i)=x(i)
    next i
    return
end sub

sub bigmul(z() as uinteger,x() as uinteger,y() as uinteger)
    if x(0)<44 or y(0)<44 then
        bigmulw(z,x,y)
        return
    end if
    dim i,M,n as integer
    if x(0)<y(0) then M=y(0) else M=x(0)
    n=M\2
    dim xl(n+1),xh(n+1),yl(n+1),yh(n+1) as uinteger
    biglow(xl,x,n):bighigh(xh,x,n)
    biglow(yl,y,n):bighigh(yh,y,n)
    dim z0(M+1),z1(M+3),z2(M+3) as uinteger
    bigadd(z0,xl,xh)
    bigadd(z2,yl,yh)
    bigmul(z1,z0,z2)
    bigmul(z0,xl,yl)
    bigmul(z2,xh,yh)
    bigsub(z1,z1,z0)
    bigsub(z1,z1,z2)
    dim imax as integer
    imax=x(0)+y(0)
    dim t,c as uinteger
    c=0
    for i=1 to imax
        t=c
        if i<=z0(0) then t=t+z0(i)
        if i>n and i-n<=z1(0) then t=t+z1(i-n)
        if i>2*n and i-2*n<=z2(0) then t=t+z2(i-2*n)
        c=0
        while t>=radix
            t=t-radix
            c=c+1
        end while
        z(i)=t
    next i
    while z(imax)=0 and imax>1
        imax=imax-1
    end while 
    z(0)=imax
    return
end sub

dim a(),b(),t1(),t2() as uinteger
sub fibowork(n as integer)
    if n=0 then
        a(0)=0:b(0)=1:b(1)=1
        return
    end if
    fibowork(n\2)
    if n mod 2=0 then
        rem [a,b]=[a*(2*b-a),b*(2*b-a)-(-1)^k]
        bigadd(t1,b,b):bigsub(t2,t1,a)
        bigmul(a,a,t2):bigmul(b,b,t2)
        if n mod 4=0 then bigdec(b) else biginc(b)
    else
        rem [a,b]=[a*(2*a+b)+(-1)^k,b*(2*a+b)]
        bigadd(t1,a,a):bigadd(t2,t1,b)
        bigmul(b,b,t2):bigmul(a,a,t2)
        if n mod 4=1 then biginc(a) else bigdec(a)
    end if
    return
end sub

sub fibo(n as integer)
    if n<2 then
        b(0)=1:b(1)=n
        return
    end if
    fibowork((n-1)\2)
    if n mod 2=0 then
        rem b=b*(2*a+b)
        bigadd(t1,a,a):bigadd(t2,t1,b):bigmul(b,b,t2)
    else
        rem b=b*(2*b-a)-(-1)^k
        bigadd(t1,b,b):bigsub(t2,t1,a):bigmul(b,b,t2)
        if n mod 4=1 then bigdec(b) else biginc(b)
    end if
    return
end sub

function main() as integer
    dim n as integer
    n=4784969
'    n=7499
    rexp=9
    cmult=5
    radix=10^rexp
    cadd=radix:cadd=cadd*cmult
    cmax=cadd*radix
    nmax=250100
    redim a(nmax),b(nmax),t1(nmax),t2(nmax),xy(nmax)
    fibo(n)
    bigprint(b)
    return 0
end function 

end module

