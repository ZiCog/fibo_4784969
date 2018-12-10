# Solutions to the million digit Fibonacci number challenge in c

## fibonacci.c

By Raspberry Pi forum member ejolson:
https://www.raspberrypi.org/forums/viewtopic.php?f=62&t=227343&start=375#p1400050
https://www.raspberrypi.org/forums/viewtopic.php?f=62&t=227343&start=375#p1400121

Uses a hand code kartasuba multiplication algorithm and runs in about a minute on a raspberry pi.

    $ gcc -O3 -ffast-math -o fibonacci fibonacci.c -lm
    $ time ./fibonacci | head -c 32
    10727395641800477229364813596225
    real    1m2.800s
    user    1m2.620s
    sys     0m0.120s
    $ time ./fibonacci | tail -c 32
    4856539211500699706378405156269

    real    1m3.034s
    user    1m2.840s
    sys     0m0.120s

Or about 2 seconds on an x86 PC

    $ time ./fibonacci | head -c 32
    10727395641800477229364813596225
    real    0m1.854s
    user    0m1.813s
    sys     0m0.031s
    michael@monster:/mnt/c/Users/michael/Documents/fibo_4784969/c$ time ./fibonacci | tail  -c 32
    4856539211500699706378405156269

    real    0m1.823s
    user    0m1.750s
    sys     0m0.094s
