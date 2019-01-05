100 rem integer.bas -- Compute the nth Fibonacci Number
110 rem Written December 25, 2018 by Eric Olson
120 rem
130 rem This program demonstrates the expressiveness of the original
140 rem versions of Microsoft BASIC as measured by explicitly coding
150 rem Karatsuba multiplication for big-number arithmetic and then
160 rem using the doubling formula
170 rem
180 rem     F(2k) = F(k)[2F(k+1)-F(k)]
190 rem   F(2k+1) = F(k+1)[2F(k+1)-F(k)]+(-1)^(k+1)
200 rem   F(2k+1) = F(k)[2F(k)+F(k+1)]+(-1)^(k)
210 rem   F(2k+2) = F(k+1)[2F(k)+F(k+1)]
220 rem
221 rem to compute the nth Fibonacci number.
222 rem
223 rem Version 2:  Minor changes to optimize the O(n^2) multiply and
224 rem prevent overflow when running on MITS BASIC.
225 rem
226 rem Version 3:  Conversion to integer data types and removal of
227 rem subroutine that determines maximum floating-point integer.
228 rem
229 rem To run this program on early versions of Microsoft BASIC please
230 rem set the default data type to 16-bit integers using defint a-z in
231 rem line 241 and edit line 260 to read b8=100:b9=2:b7=32768-2*b8*b8.
232 rem Then change n so the resulting Fibonacci number fits in memory.
240 rem
241 deflng a-z
242 rem defint a-z
250 n=4784969
251 rem n=7499
260 b8=10000:b9=4:b7=2147483647-2*b8*b8
261 rem b8=100:b9=2:b7=32768-2*b8*b8
265 b6=b7\b8:b7=b8*b6
270 d9=int(n*log((1+sqr(5))/2)/log(b8)+7):m9=14
280 dim m(d9*m9):m0=1
300 a=m0:m0=m0+1+d9:b=m0:m0=m0+1+d9
310 t1=m0:m0=m0+1+d9:t2=m0:m0=m0+1+d9
400 r0=n:gosub 1000
420 r7=b:gosub 7000
430 stop
1000 rem Compute nth Fibonacci number
1010 rem    inputs: r0 the value of n
1020 rem   outputs: b the value of F(n)
1040 if r0<2 then m(b)=1:m(b+1)=r0:return
1060 n1=r0:r0=(n1-1)\2:gosub 1500
1070 p1=n1 mod 4
1080 if p1=1 or p1=3 goto 1200
1090 r1=t1:r2=a:r3=a:gosub 2000
1110 r1=t2:r2=t1:r3=b:gosub 2000
1120 r1=t1:r2=b:r3=t2:gosub 4000
1170 r1=b:r2=t1:gosub 5000:return
1200 r1=t1:r2=b:r3=b:gosub 2000
1210 r1=t2:r2=t1:r3=a:gosub 3000
1220 r1=t1:r2=b:r3=t2:gosub 4000
1230 if p1=3 then 1250
1240 r1=t1:gosub 6000:goto 1260
1250 r1=t1:gosub 6500
1260 r1=b:r2=t1:gosub 5000:return
1500 rem Recursive work for nth Fibonacci number
1510 rem    inputs: r0 the value of n
1520 rem   outputs: a the value of F(n)
1530 rem   outputs: b the value of F(n+1)
1540 if r0=0 then m(a)=1:m(a+1)=0:m(b)=1:m(b+1)=1:return
1600 m(m0)=r0:m0=m0+1:r0=r0\2:gosub 1500
1610 m0=m0-1:r0=m(m0)
1620 p1=r0 mod 4
1630 if p1=1 or p1=3 then 1720
1640 r1=t1:r2=b:r3=b:gosub 2000
1650 r1=t2:r2=t1:r3=a:gosub 3000
1660 r1=t1:r2=a:r3=t2:gosub 4000
1670 r1=a:r2=t1:gosub 5000
1680 r1=t1:r2=b:r3=t2:gosub 4000
1690 if p1=2 then 1710
1700 r1=t1:gosub 6000:goto 1711
1710 r1=t1:gosub 6500
1711 r1=b:r2=t1:gosub 5000:return
1720 r1=t1:r2=a:r3=a:gosub 2000
1730 r1=t2:r2=t1:r3=b:gosub 2000
1740 r1=t1:r2=b:r3=t2:gosub 4000
1750 r1=b:r2=t1:gosub 5000
1760 r1=t1:r2=a:r3=t2:gosub 4000
1770 if p1=3 then 1790
1780 r1=t1:gosub 6500:goto 1800
1790 r1=t1:gosub 6000
1800 r1=a:r2=t1:gosub 5000:return
2000 rem Big-number addition
2010 rem  outputs: r1 the value of a+b
2020 rem   inputs: r2 the value of a
2030 rem   inputs: r3 the value of b
2050 if m(r3)>m(r2) then i9=m(r3) else i9=m(r2)
2060 for i=1 to i9+1:m(r1+i)=0:next i
2070 for i=1 to i9:c=0:t=m(r1+i)
2080 if i<=m(r2) then t=t+m(r2+i)
2090 if i<=m(r3) then t=t+m(r3+i)
2110 if t>=b8 then c=1:t=t-b8
2120 m(r1+i)=t:m(r1+i+1)=m(r1+i+1)+c:next i
2130 m(r1)=i9+1
2140 r4=r1:gosub 7500
2150 return
3000 rem Big-number subtraction
3010 rem  outputs: r1 the value of a-b
3020 rem   inputs: r2 the value of a
3030 rem   inputs: r3 the value of b
3050 for i=1 to m(r2):m(r1+i)=0:next i
3060 for i=1 to m(r3):t=m(r1+i)+m(r2+i)-m(r3+i)
3070 if t<0 then t=t+b8:m(r1+i+1)=m(r1+i+1)-1
3080 m(r1+i)=t:next i
3090 for i=m(r3)+1 to m(r2):t=m(r1+i)+m(r2+i)
3100 if t<0 then t=t+b8:m(r1+i+1)=m(r1+i+1)-1
3110 m(r1+i)=t:next i
3120 m(r1)=m(r2)
3130 r4=r1:gosub 7500
3150 return
4000 rem Big-number multiplication
4010 rem  outputs: r1 the value of a*b
4020 rem   inputs: r2 the value of a
4030 rem   inputs: r3 the value of b
4040 if m(r2)>80 and m(r3)>80 then 4300
4050 i9=m(r2)+m(r3):for i=1 to i9:m(r1+i)=0:next i
4070 for i=1 to m(r2):for j=1 to m(r3)
4080 t=m(r1+i+j-1)+m(r2+i)*m(r3+j)
4090 if t<b7 then 4120
4100 m(r1+i+j-1)=t-b7
4110 m(r1+i+j)=m(r1+i+j)+b6:goto 4130
4120 m(r1+i+j-1)=t
4130 next j:next i
4140 c=0:for i=1 to i9:t=m(r1+i)+c
4150 if t<b8 then c=0:goto 4170
4160 c=t\b8:t=t mod b8
4170 m(r1+i)=t:next i
4180 m(r1)=i9
4190 r4=r1:gosub 7500
4230 return
4300 rem Big-number Karatsuba algorithm
4310 if m(r2)<m(r3) then i8=m(r3) else i8=m(r2)
4320 i8=i8\2
4330 z0=m0:m0=m0+1+2*i8+1
4332 z2=m0:m0=m0+1+2*i8+3
4334 z1=m0:m0=m0+1+2*i8+5
4340 z3=m0:m0=m0+1+i8+2
4350 z4=m0:m0=m0+1+i8+2
4360 r5=z4:r6=r3:gosub 4500
4370 r5=z3:r6=r2:gosub 4500
4380 gosub 4600:r1=z1:r2=z3:r3=z4:gosub 4000:gosub 4700
4400 q1=m(r2):if i8<q1 then m(r2)=i8
4405 q2=m(r3):if i8<q2 then m(r3)=i8
4410 gosub 4600:r1=z0:gosub 4000:gosub 4700
4420 m(r2)=q1:m(r3)=q2
4430 q3=q1-i8:q4=q2-i8:if q3<0 or q4<0 then m(z2)=0:goto 8000
4440 q1=m(r2+i8):m(r2+i8)=q3:q2=m(r3+i8):m(r3+i8)=q4
4450 gosub 4600:r1=z2:r2=r2+i8:r3=r3+i8:gosub 4000:gosub 4700
4460 m(r2+i8)=q1:m(r3+i8)=q2
4470 goto 8000
4500 rem Add high to low
4510 rem  outputs: r5 the sum of high(a)+low(a)
4520 rem   inputs: r6 the value of a
4530 rem   inputs: i8 the split point
4540 c=0:for i=1 to i8+1:t=c
4545 if i<=i8 and i<=m(r6) then t=t+m(r6+i)
4550 if i+i8<=m(r6) then t=t+m(r6+i+i8)
4560 if t>=b8 then c=1:t=t-b8 else c=0
4570 m(r5+i)=t:next i:m(r5+i8+2)=c
4590 m(r5)=i8+2:r4=r5:gosub 7500
4595 return
4600 rem Save frame
4610 m(m0)=z1:m0=m0+1:m(m0)=z2:m0=m0+1:m(m0)=z0:m0=m0+1
4620 m(m0)=i8:m0=m0+1:m(m0)=q1:m0=m0+1:m(m0)=q2:m0=m0+1
4630 rem Save parameters
4640 m(m0)=r1:m0=m0+1:m(m0)=r2:m0=m0+1:m(m0)=r3:m0=m0+1
4650 return
4700 rem Restore frame
4710 gosub 4750
4720 m0=m0-1:q2=m(m0):m0=m0-1:q1=m(m0):m0=m0-1:i8=m(m0)
4730 m0=m0-1:z0=m(m0):m0=m0-1:z2=m(m0):m0=m0-1:z1=m(m0)
4740 return
4750 rem Restore parameters
4760 m0=m0-1:r3=m(m0):m0=m0-1:r2=m(m0):m0=m0-1:r1=m(m0)
4770 return
5000 rem Big-number copy
5010 rem  outputs: r1 the value of a
5020 rem   inputs: r2 the value of a
5030 r4=r2:gosub 7500
5040 for i=1 to m(r2):m(r1+i)=m(r2+i):next i
5050 for i=m(r2)+1 to m(r1):m(r1+i)=0:next i
5060 m(r1)=m(r2)
5070 return
6000 rem Big-number decrement
6010 rem   inputs: r1 the value of a
6020 rem  outputs: r1 the value of a-1
6040 i=1:c=1
6050 if c=0 then 6080
6060 if m(r1+i)<1 then m(r1+i)=b8-1 else m(r1+i)=m(r1+i)-1:c=0
6070 i=i+1:goto 6050
6080 r4=r1:gosub 7500
6100 return
6500 rem Big-number increment
6510 rem   inputs: r1 the value of a
6520 rem  outputs: r1 the value of a+1
6540 m(r1)=m(r1)+1:m(r1+m(r1))=0:i=1:c=1
6550 if c=0 then 6590
6560 t=m(r1+i)+1
6570 if t>=b8 then m(r1+i)=t-b8 else m(r1+i)=t:c=0
6580 i=i+1:goto 6550
6590 r4=r1:gosub 7500
6600 return
7000 rem Big-number print
7010 rem   inputs: r7 the value to print
7020 if m(r7)=0 then print "0":return
7030 for i=m(r7) to 1 step -1
7040 s$=str$(m(r7+i))
7045 if mid$(s$,1,1)=" " then s$=mid$(s$,2,len(s$)-1)
7050 if i=m(r7) or b9<=len(s$) then 7070
7060 s$=mid$("0000000000000",1,b9-len(s$))+s$
7070 print s$;:next i:print:return
7500 rem Big-number trim
7510 rem    inputs: r4 the value of a
7520 rem   outputs: r4 the trimmed value of a
7530 if m(r4+m(r4))=0 and m(r4)>1 then m(r4)=m(r4)-1:goto 7530
7540 return
8000 rem Tail of Karatsuba
8003 t4=m0-1-2*i8-5
8005 gosub 4630:r1=t4:r2=z1:r3=z0:gosub 3000
8010 r1=z1:r2=t4:r3=z2:gosub 3000:gosub 4750
8020 r4=z1:gosub 7500
8030 i9=m(r2)+m(r3):i7=2*i8:c=0:for i=1 to i9:t=c
8040 if i<=m(z0) then t=t+m(z0+i)
8050 if i>i8 and i-i8<=m(z1) then t=t+m(z1+i-i8)
8060 if i>i7 and i-i7<=m(z2) then t=t+m(z2+i-i7)
8070 if t<b8 then c=0:goto 8090
8080 c=1:t=t-b8
8090 m(r1+i)=t:next i
8092 m(r1)=i9:r4=r1:gosub 7500
8100 m0=z0
8120 return
9999 end