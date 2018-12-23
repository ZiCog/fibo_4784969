# The 4784969th Fibonacci number in FreeBASIC

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

