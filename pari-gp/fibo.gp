/*
	fibo.gp - calculated big float Fibonacci(n) in PARI/GP
		    -- https://pari.math.u-bordeaux.fr/ --
		  with a million digits of precision
	scruss - 2019-05

	To run (with 128 MB of stack):

		gp -s $((128*1024**2)) -q fibo.gp

	To install on a Debian system:

		sudo apt install pari-gp
		(this seems to install all of TeXLive too, sorry)

	*** NB: Output is seemingly (?) always in scientific notation
		and includes header on setting precision

	    Also, we're avoiding gp's fibonacci(n) function as it
	    	uses gmp.

	Timing (Raspberry Pi 3B+):

time gp -s $((128*1024**2)) -q fibo.gp |tail -1 | head -c 33 ; time gp -s $((128*1024**2)) -q fibo.gp |tail -1 | tail -c 40

1.0727395641800477229364813596225
real	0m9.845s
user	0m9.723s
sys	0m0.119s
4856539211500699706378405156269 E999999

real	0m9.844s
user	0m9.729s
sys	0m0.110s
*/

\p 1000000			            /* set precision to 1,000,000 digits */
phi = (1 + sqrt(5)) / 2		  /* Golden Ratio */
fibo(n) = (phi^n) / sqrt(5)
print(fibo(4784969))
quit
