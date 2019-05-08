# The 4784969th Fibonacci number in Julia

Author: heater

Origin: https://www.raspberrypi.org/forums/viewtopic.php?f=62&t=227343&start=2275#p1463945

Post:

Julia was my first girl friend. Now I find she can do this:

    $ time ./fibo.julia | head -c 32
    10727395641800477229364813596225
    real    0m0.885s
    user    0m0.688s
    sys     0m0.703s
    $ time ./fibo.julia | tail  -c 32
    4856539211500699706378405156269
    real    0m0.863s
    user    0m0.641s
    sys     0m0.641s
    $




