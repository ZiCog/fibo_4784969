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

// Derived from Paeryn's Haslell FiboFast
function fiboFast (n) {
  if (typeof memo[n] != 'undefined') {
    return memo[n]
  }

  let k = (n / 2)|0
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

let a = BigInt(0)
let b = BigInt(1)

function fibowork(n) {
  if(n == 0) {
      a = BigInt(0)
      b = BigInt(1)
      return
  }
  fibowork((n/2)|0, a , b)
  if (n % 2 === 0 ) {
      // [a,b]=[a*(2*b-a),b*(2*b-a)-(-1)^k]
      t1 = 2n * b
      t2 = t1 - a
      t1 = a * t2
      if (n % 4 == 0) {
        b = (b * t2) - 1n
      } else{
        b = (b * t2) + 1n
      }
      a = t1;
  } else {
      // [a,b]=[a*(2*a+b)+(-1)^k,b*(2*a+b)]
      t1 = 2n * a
      t2 = t1 + b
      t1 = b * t2
      if(n % 4 === 1) {
        a = (a * t2) + 1n
      } else {
        a = (a * t2) - 1n
      }
      b = t1
  }
  return
}

// Derived from ejolson's fibogmp
function fiboFaster(n) {
  if(n < 2) {
      b = BigInt(n)
      return b
  }
  fibowork(((n-1)/2)|0, a, b);
  if(n % 2 === 0) {
      // b=b*(2*a+b)
      b = b * ((2n * a) + b)
  } else {
      // b=b*(2*b-a)-(-1)^k
      if (n % 4 === 1) {
        b = (b * ((2n * b) - a)) - 1n
      }
      else {
        b = (b * ((2n * b) - a)) + 1n
      }
  }
  return b
}

let res

function timeIt (f, k) {
  t = new Date()
  res = f(k)
  dur = new Date() - t
  console.log(dur + "ms")
}

//timeIt(fibo, 4784969)
//timeIt(fiboFast, 4784969)
//console.log(res.toString(10))

timeIt(fiboFaster, 4784969)
console.log(res.toString(10))

