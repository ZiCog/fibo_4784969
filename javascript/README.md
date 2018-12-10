# Two solutions to the million digit Fibonacci number challenge in javascript

## fibo.js

By heater. Uses javascript the BigInt type.

The fibo calculation takes a mere 12 seconds on an x86 PC. But producing the million decimal digit output takes almost a minute!

    $ time node fibo.js
    12766ms
    ....
    152076875379936330930391815964864885353407167474856539211500699706378405156269

    real    0m58.158s
    user    0m57.766s
    sys     0m0.172s

## fibo_karatsuba.js

By heater on the Raspberry Pi forum https://www.raspberrypi.org/forums/viewtopic.php?f=62&t=227343&start=500#p1402101

Uses a hand made karatsuba big integer multiplication algorith rather than the BigInt type of recent JS standards. Mostly as just an exercise in creating a karatuba.

Runs on an x86 PC in some minutes..

    $ time node karatsuba.js > fibo_karatsuba.dat
    real    3m23.488s
    user    3m19.469s
    sys     0m12.797s
    $ head -c 32 fibo_4784969.dat
    10727395641800477229364813596225
    $ tail -c 32 fibo_4784969.dat
    4856539211500699706378405156269

