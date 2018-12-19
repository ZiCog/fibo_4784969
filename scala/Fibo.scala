object Fibo {

    def fibo (n : Int) : BigInt = {
        if (n == 0) {
            return BigInt(0)
        }
        if (n <= 2) {
            return BigInt(1)
        }
        var k = (n / 2)
        var fk = fibo(k)
        var fk1 = fibo(k + 1)
        if ((n & 1) == 0)
        {
            return fk * (fk1 * BigInt(2) - fk)
        }
        return (fk * fk) + (fk1 * fk1)
    }

    def main(args: Array[String]): Unit = {
        println(fibo(4784969))
    }
}

