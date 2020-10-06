Million digit Fibonacci numbers in Scheme.

---------------------

File: fibo.racket.scm

Dialect: Racket Scheme

Author: Unknown ?

Origin: https://www.raspberrypi.org/forums/viewtopic.php?f=62&t=227343&start=1850#p1458461

-----------------

File: fibo.chez.scm

Dialect: Guile, Chez

Author: Unknown ?

Origin: Derived from fibo.racket.scm by Heater.

This has memozation removed and the Racket specific boiler plate, square brackets, etc.  

    $ time scheme --script fibo.chez.scm | head -c 32
    10727395641800477229364813596225

    real    0m13.588s
    user    0m13.063s
    sys     0m0.125s


-----------------

File: fibo.chibi.scm

Dialect: Chibi Scheme

Author: Unknown ?

Origin: Derived from fibo.racket.scm by Heater.

This has Racket specific boiler plate, square brackets, etc removed  
Added import statements.
Changed memoization to use SRFI 69 hast tables.

This is slow. The memoization shaves nearly a whole minute of it's run time!

    $ time chibi-scheme fibo.chibi.scm | head -c 32
    10727395641800477229364813596225
    real    15m21.221s
    user    15m20.469s
    sys     0m0.078s



-----------------------

Notes:

    From the Racket Style Guide : https://docs.racket-lang.org/style

    6.1 Where to Put Parentheses
        Racket isn’t C. Put all closing parentheses on one line, the last line of your code.

Stuff that nonsense. I need to pull those brackets appart to have any chance to see what is going on.








