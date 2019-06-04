#!/usr/bin/env python
import sys

import math
from decimal import *


if len(sys.argv) != 2:
  print("usage:\n\t Fibo  Fibonacci_number")
  quit()


value = int(sys.argv[1])


if value <= 0:
   print("The value need to be > 0");
   quit()
if value <3:
  print("1")
  quit()

#let's figure out the number of digits needed

nDigits = int(round(math.log10(value))*2)


#let's calculate a approx phi

phi = (Decimal(1) + Decimal.sqrt(Decimal(5))) / Decimal(2)

Fn = (phi ** Decimal(value)) / Decimal.sqrt(Decimal(5))

precision = int(Decimal.to_integral_value(Decimal.log10(phi ** Decimal(value))) + nDigits )

print(precision)
if precision >  getcontext().prec:
    getcontext().prec = precision
    #ok let;s recalculate phi with the correct precision
    sqrt5 = Decimal.sqrt(Decimal(5))
    phi = (Decimal(1) + sqrt5) / Decimal(2)
    Fn = (phi ** Decimal(value)) / sqrt5

Fn = Decimal.to_integral_value(Fn)
print(Fn)

