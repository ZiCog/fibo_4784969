# Solutions to the million digit Fibonacci number challenge in Haskell

## fibo.hs

By Raspberry Pi forum member Paeryn:
https://www.raspberrypi.org/forums/viewtopic.php?f=62&t=227343&start=825#p1407243

This is blindingly fast. There are three progressively faster methods included in the source, the fastest one run out of the box.

Get yourself a Haskell compiler, for example on Debian:

    $ sudo apt-get install ghc

Build and run it (timed on my x86 PC):

    $ ghc fibo.hs
    $ time ./fibo | head -c 32 ;  time ./fibo | tail  -c 32 ;

    $ time ./fibo | head -c 32 ;  time ./fibo | tail  -c 32 ;
    10727395641800477229364813596225
    real    0m0.143s
    user    0m0.063s
    sys     0m0.063s
    4856539211500699706378405156269

