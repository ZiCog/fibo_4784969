# The 4784969th Fibonacci number in BASIC

## classic.bas

classic.bas is a fibo(4784969) calculator written in classic style line numbered BASIC. Should be runnable on any 
8 bit BASICs and others (Provided one takes the fibo of smaller numbers of course)

By ejolson in this forum post https://www.raspberrypi.org/forums/viewtopic.php?f=62&t=227343&start=975#p1411197

Build and run with FreeBASIC using the qb language option: 

    $ fbc -O 3 -lang qb -fpu sse -fpmode fast classic.bas
    michael@monster:/mnt/c/Users/michael/Documents/fibo_4784969/FreeBASIC$ time ./classic | head -c 32 ; time ./classic | tail -c 32
    10727395641800477229364813596225
    real    0m24.902s
    user    0m5.797s
    sys     0m19.094s
    4856539211500699706378405156269

    real    0m26.359s
    user    0m6.766s
    sys     0m20.281s

## fibo.bas

fibo.bas is a fibo(4784969) calculator written for FreeBASIC 

By ejolson in this forum post https://www.raspberrypi.org/forums/viewtopic.php?f=62&t=227343&start=450


As this program is using the O(n^2) multiplication throughout it runs noticeably slower than codes using asymptotically superior algorithms. Run times on the original Pi 2B are as follows:

    $ time ./fibo | head -c32; time ./fibo | tail -c32
    10727395641800477229364813596225
    real    36m41.437s
    user    36m40.874s
    sys 0m0.149s
    4856539211500699706378405156269

    real    36m14.462s
    user    36m11.622s
    sys 0m3.141s


In my opinion, FreeBASIC is better than using Cfront to code in C++ and just as much fun as using Bourne's macros to turn C into Algol.

To get FreeBASIC running on the Raspberry Pi, I downloaded the most recent linux-armv7a-hf-debian binary from here, made the directory /usr/local/fbc-299 for safe keeping and unzipped the archive. After installing libncurses5-dev I could compile with the command

    $ /usr/local/fbc-299/bin/fbc -O 3 fibo.bas


UPDATE: 23 Dec 2018

A new version by ejolson dramically improves run time:
https://www.raspberrypi.org/forums/viewtopic.php?f=62&t=227343&start=825#p1407254


    $ fbc -O 3 fibo.bas
    $ time ./fibo | head -c 32 ; time ./fibo | tail -c 32
    10727395641800477229364813596225
    real    0m24.314s
    user    0m24.156s
    sys     0m0.031s
    4856539211500699706378405156269

    real    0m25.414s
    user    0m24.469s
    sys     0m2.188s

# Visual Basic

Author: ejolson

Origin: https://www.raspberrypi.org/forums/viewtopic.php?p=1418137#p1418137

Post:

I have finished writing visual.bas, a version of the Fibonacci code in Visual Basic. This program is based on the FreeBASIC fibo.bas program with syntax modifications so it will compile and the addition of the missing Karasuba algorithm. The program was compiled using the open source vbnc compiler running in the mono environment with the optimizer turned on and integer range checking turned off. The best out of three runs are reported here:
Code: Select all

Pi Zero timings of the Fibonacci code

                   BUILD TIME    RUN TIME     TOTAL
serial                7.459       24.195     31.654
fibonacci             5.060       80.946     86.006
fibo_karatserial     56.154       40.090     96.244
integervol           17.886       87.556    105.442
visual               14.146      164.411    177.557
classic              10.851      217.004    227.855

    $ time vbnc /optimize /removeintchecks visual.bas
    $ time mono visual.exe >visual.txt

It should be noted that visual.bas is not compiled into a native ARM executable but instead into some sort of machine-independent intermediate code. This intermediate code is subsequently interpreted by the mono just-in-time compiler. It would be interesting to know how Microsoft's implementation of Visual Basic compares in terms of efficiency. Could one compile visual.bas using the official Microsoft compiler and then run the resulting intermediate code under mono on Raspbian? What about the other way around? How do the two implementations of Visual Basic compare on x86?

For reference the code is... visual.bas
