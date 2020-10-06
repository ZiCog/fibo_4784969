//
// Fibo.java
//
// Calculate the 4784969th number in the Fibonnaci sequence. The first Fibonacci number
// with one million digits.
//
import java.math.BigInteger; 

public class Fibo 
{ 
	// Returns Factorial of N 
	// Derived from Paeryn's Haskell FiboFast
	static BigInteger fibo(int n) 
	{
 		if (n == 0) return new BigInteger("0");
		if (n == 1) return new BigInteger("1");
		BigInteger TWO = new BigInteger("2");
		int k = (n / 2);
		BigInteger fk = fibo(k);
		BigInteger fk1 = fibo(k - 1);
		if ((n & 1) == 0)
		{
			return fk.multiply(TWO.multiply(fk1).add(fk));
		}
		BigInteger twoA = TWO.multiply(fk);
		BigInteger c = twoA.add(fk1).multiply(twoA.subtract(fk1)); 
		if ((n % 4) == 1)
		{
			return c.add(TWO);
		}
		return c.subtract(TWO);
	} 

	public static void main(String args[]) throws Exception 
	{ 
		int n = 4784969;
		BigInteger result = fibo(n); 
		System.out.println(result);
	} 
} 
