import math
import timeit


fibs = {0:0, 1:1, 2:1}

def fibo(n):
    if n in fibs:
        return fibs[n]

    k = (n + 1) // 2
    fk = fibo(k)
    fk1 = fibo(k - 1)
    if n & 1:
        result = fk ** 2 + fk1 ** 2
    else:
        result = (2 * fk1 + fk) * fk
    fibs[n] = result
    return result


run_time = timeit.timeit('fibo(4784969)', setup="from __main__ import fibo", number=10)


print(fibo(4784969))
print('Run time = {}'.format(run_time))




