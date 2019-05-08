# The 4784969th Fibonacci number in Maxima

## Author: ejolson

## Origin: https://www.raspberrypi.org/forums/viewtopic.php?f=62&t=227343&start=2225#p1463653

## Post:

Think how different the world would be if the last hacker, instead of writing gcc and all those Unix-like utilities, had instead written a 36-bit PDP-10 emulator in Smalltalk and preserved the assembler legacy of the Incompatible Timesharing System.

If DEC hadn't cancelled the PDP-10 and Compaq hadn't cancelled DEC and HP hadn't cancelled Compaq...but that's all history and speculation. The question today is whether SoftBank will cancel ARM and whether RISC-V will ever run Smalltalk. This brings us back to the question of emulating the PDP-10 running ITS so that large Fibonacci numbers can be computed using the original version of Macsyma.

Fortunately, Macsyma is available on the Pi for trademark reasons as the open-source programming language called Maxima. The code for computing your and my favourite Fibonacci number looks like  ... fibo.max

When running on a Pi Zero the output is
    
    $ time ./fibo.sh > fibo.dat
    real    0m11.140s
    user    0m9.130s
    sys     0m1.570s
    $ head -c32 macfibo.txt; echo
    10727395641800477229364813596225

## Comment

Can aslo be run as:

    $ time maxima --very-quiet -b=fibo.max  | head -c 64

    batch("fibo.max")

    read and interpret file: #p/mnt/c/Users/heatInappropriate ioctl for device

    real    0m0.645s
    user    0m0.047s
    sys     0m0.375s
    $ time maxima --very-quiet -b=fibo.max  | tail     -c 64
    699706378405156269
                                    fibo.max

    real    0m2.888s
    user    0m0.750s
    sys     0m2.125s
    $





