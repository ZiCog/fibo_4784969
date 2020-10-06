# File

fibo_phy.py

## Author:

danjperron.

## Origin:

https://www.raspberrypi.org/forums/viewtopic.php?f=31&t=240287&start=150#p1476102

## Run

On an Intel PC this is pretty snappy:

	$ time python3 fibo_phi.py 4784969 | head -c 32 ; time python3 fibo_phi.py 4784969 | tail -c 32
	1000013
	107273956418004772293648Traceback (most recent call last):
	  File "fibo_phi.py", line 45, in <module>
	    print(Fn)
	BrokenPipeError: [Errno 32] Broken pipe
	
	real    0m15.840s
	user    0m15.125s
	sys     0m0.688s
	4856539211500699706378405156269
	
	real    0m15.628s
	user    0m15.016s
	sys     0m0.578s	


## Note

Under Python2 we have yet to wait long enough for it to finish!



