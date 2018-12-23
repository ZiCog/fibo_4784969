//
// This is a fast and big Fibonacci number calculator based on the suggestions here:
//
// https://www.nayuki.io/page/fast-fibonacci-algorithms
//
function isEven(n) {
  return (n & 1) === 0;
}

let memo = [BigInt(0), BigInt(1), BigInt(1)]

function fibo (n) {
  if (memo[n]) {
    return memo[n]
  }
  let k = (n / 2) | 0
  let a = fibo(k);
  let b = fibo(k + 1);
  if (isEven(n)) {
    return memo[n] = a * ((b * 2n) - a)
  }
  return memo[n] = a ** 2n + b ** 2n
}

function fiboFast (n) {
  if (typeof memo[n] != 'undefined') {
    return memo[n]
  }

  let k = (n / 2) | 0
  let a = fiboFast(k)
  let b = fiboFast(k - 1)
  if (isEven(n)) {
    return memo[n] = a * (2n * b + a)
  }
  if ((n % 4) === 1) {
    return memo[n] = (2n * a + b) * (2n * a - b) + 2n
  }
  return memo[n] = (2n * a + b) * (2n * a - b) - 2n
}

let res

function timeIt (f, k) {
  t = new Date()
  res = f(k)
  dur = new Date() - t
  console.log(dur + "ms")
}

//timeIt(fibo, 4784969)
timeIt(fiboFast, 4784969)
console.log(res.toString(10))
