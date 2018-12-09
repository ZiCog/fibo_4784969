//
// Karatsaba
//
// Pseudo code from: https://en.wikipedia.org/wiki/Karatsuba_algorithm
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

let BASE = 10
let LIMIT = BASE - 1

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
        if (isNaN(b[i])) {
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
    console.log('carry = ', carry)
    if (carry) {
        console.log('sub: Nobody expects a negative result!')
        console.log(a)
        console.log(b)
        process.exit(1)
    }
    return result
}

function scalarMul(vector, scalar) {
    let result = []
    carry = 0
    for (let i = 0; i < vector.length; i++) {
        let r = vector[i] * scalar + carry
        if (r < BASE) {
            result[i] = r;
            carry = 0
        }
        else
        {
            carry = (r / BASE)|0;
            result[i] = (r % 10)|0;
        }
    }
    return result
}

function shift(a, m) {
    return Array(m).fill(0).concat(a)
}

function karatsuba(a, b) {
    let result
    if (a.length === 1) {
        result = scalarMul(b, a[0])
    } else if (b.length === 1) {
        result = scalarMul(a, b[0])
    } else {
        console.log("Will do bigint multiply")

        let m = a.length < b.length ? a.length : b.length
        console.log('m = ', m)
        let m2 = (m/2)|0
        console.log('m2 = ', m2)

        let high1 = a.slice(m2, a.length)
        let low1 = a.slice(0, m2)
        console.log('low1', low1, 'high1', high1)

        let high2 = b.slice(m2, b.length)
        let low2 = b.slice(0, m2)
        console.log('low2', low2, 'high2', high2)

        let z0 = karatsuba(low1, low2)
        console.log('z0', z0)
        let z1 = karatsuba(sum(low1, high1), sum(low2, high2))
        console.log('z1', z1)
        let z2 = karatsuba(high1, high2)
        console.log('z2', z2)

        let s1 = sub(z1, z2)
        console.log('s1', s1)
        let s2 = sub(s1, z0)
        console.log('s2', s2)
//        return (z2 * 10 ^ (m2 * 2)) + ((z1 - z2 - z0) * 10 ^ m2) + z0
        result = sum(sum(shift(z2, m2 * 2), shift(s2, m2)), z0)
    }
    return result
}

let a = [1, 1, 1, 1]
let b = [1, 1, 1, 1]

let k = karatsuba(a, b)
console.log(k)






