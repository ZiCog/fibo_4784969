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
    let res = memo[n]
    return res
  }
  let k = (n / 2) | 0
  if (isEven(n)) {
    let fk = fibo(k);
    let res = memo[n] = fk * ((fibo(k + 1) * 2n) - fk)
    return res
  }
  let res = memo[n] = fibo(k) ** 2n + fibo(k + 1) ** 2n
  return res
}

let res

function timeIt (f, k) {
  t = new Date()
  res = f(k)
  dur = new Date() - t
  console.log(dur + "ms")
}

timeIt(fibo, 4784969)
console.log(res.toString(10))














