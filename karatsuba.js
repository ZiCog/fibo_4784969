//
// Karatsaba multiply: pseudo code from: https://en.wikipedia.org/wiki/Karatsuba_algorithm
//
//    procedure karatsuba(num1, num2)
//        if (num1 < 10) or (num2 < 10)
//            return num1*num2
//
//        // Calculates the size of the numbers
//        m = min(size_base10(num1), size_base10(num2))
//        m2 = floor(m/2)
//
//        // Split the digit sequences in the middle
//        high1, low1 = split_at(num1, m2)
//        high2, low2 = split_at(num2, m2)
//
//        // 3 calls made to numbers approximately half the size
//        z0 = karatsuba(low1, low2)
//        z1 = karatsuba((low1 + high1), (low2 + high2))
//        z2 = karatsuba(high1, high2)
//        return (z2 * 10 ^ (m2 * 2)) + ((z1 - z2 - z0) * 10 ^ m2) + z0
//

let DIGITS = 7                  // Decimal digits in each big integer array element.
let BASE = Math.pow(10, DIGITS)

function sum(x, y)
{
    let a
    let b

    // Get operands, a being the shorter
    if (x.length > y.length) { 
        a = x
        b = y
    } else {
        a = y
        b = x
    }

    let len = a.length
    let sum = []
    let carry = 0

    for (let i = 0; i < len; i++) {
    
        // Sum the elements with carry
        let s
        if (b[i] === undefined) {
            s = a[i]
        } else {
            s = a[i] + b[i]
        }       
        s += carry

        // Handle overflow and generate carry
        if (s >= BASE) {
          carry = 1;
          s -= BASE;
        } else {
          carry = 0;
        }

        // Stash result
        sum[i] = s
    }
    // If there is a carry append it to the sum array
    if (carry === 1)
    {
        sum.push(1);
    }
    return sum
}

// Assume that a > b
function sub(a, b) {
    result = []
    let carry = 0;
    for (let i = 0; i < a.length; i++) {
        let x = a[i]
        let y = b[i]

        if (isNaN(y)) {
            y = 0
        }

        if (carry) {
            x--;
        }

        let s;
        if (x < y) {
            s = x + BASE - y
            carry = 1
        } else {
            s = x - y
            carry = 0
        }
        result[i] = s
    }
    return result
}

function simpleMul(a, k) {
    let result = []
    carry = 0
    for (let i = 0; i < a.length; i++) {
        let r = a[i] * k + carry
        if (r < BASE) {
            result[i] = r;
            carry = 0
        }
        else
        {
            carry = Math.floor(r / BASE);
            result[i] = Math.floor(r % BASE);
        }
    }
    if (carry) {
        result.push(carry)
    }
    return result
}

function shift(a, m) {
    return Array(m).fill(0).concat(a)
}

function mul(a, b) {
    let result
    if (a.length === 1) {
        result = simpleMul(b, a[0])
    } else if (b.length === 1) {
        result = simpleMul(a, b[0])
    } else {
        let m = a.length < b.length ? a.length : b.length
        let m2 = (m/2)|0

        let high1 = a.slice(m2, a.length)
        let low1 = a.slice(0, m2)
        let high2 = b.slice(m2, b.length)
        let low2 = b.slice(0, m2)

        let z0 = mul(low1, low2)
        let z1 = mul(sum(low1, high1), sum(low2, high2))
        let z2 = mul(high1, high2)

        let s1 = sub(z1, z2)
        let s2 = sub(s1, z0)

        result = sum(sum(shift(z2, m2 * 2), shift(s2, m2)), z0)
    }
    return result
}

function printBigInteger (k) {
    let digits = "" + k[k.length - 1]
    process.stdout.write(digits); 
    for (let i = k.length - 2; i >= 0; i--) {
        let digits = "" + k[i]
        process.stdout.write(digits.padStart(DIGITS, '0')); 
    }
    process.stdout.write("\n"); 
}

function isEven(n) {
    return (n & 1) === 0;
}
  
let zero = [0];
let one = [1];
let two = [2];

let memo = [zero, one, one]
  
function fibo (n) {
    if (memo[n]) {
        let res = memo[n]
        return res
    }
    let k = (n / 2) | 0
    let fk = fibo(k)
    let fk1 = fibo(k + 1)
    if (isEven(n)) {
        let res = memo[n] = mul(fk, sub(mul(fk1, two,), fk))
        return res
    }
    let res = memo[n] = sum(mul(fk, fk), mul(fk1, fk1))
    return res
}

r = fibo(4784969)
printBigInteger(r)





