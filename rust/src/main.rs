//! A program to compute the first one-million digits Fivonacci number
//!
//! This program computes the first Fibonacci number consisting of one million decimal digits,
//! which happend to be F<sub>4784969</sub>. It is based upon the doubling formulae from the
//! FreeBasic version by Eric Olson:
//! * (F<sub>2k</sub>, F<sub>2k+1</sub>)
//!   = (F<sub>k</sub>[2F<sub>k+1</sub>-F<sub>k</sub>], F<sub>k+1</sub>(2F<sub>k+1</sub>-F<sub>k</sub>] + (-1)<sup>k+1</sup>)
//! * (F<sub>2k+1</sub>, F<sub>2k+2</sub>)
//! = (F<sub>k</sub>[2F<sub>k</sub>+F<sub>k+1</sub>] + (-1)<sup>k</sup>, F<sub>k+1</sub>[2F<sub>k</sub>+F<sub>k+1</sub>])
//!
//! This program utilises the extremely fast [ibig](https://crates.io/crates/ibig) library written
//! by Tomek Czajka. The canonical solution for big integer arithmetic would probably be
//! [num-bigint](https://crates.io/crates/num-bigint), but while that crate is certainly able to
//! *compute* the required number in a few tenths of seconds, converting the result to decimal form
//! for printing takes a whopping 21 seconds on a Core(TM) i7-6700HQ laptop.
//!
//! # Compilation
//!
//! `cargo build --release`
//!
//! # Run
//!
//! ```text
//! cargo run --release
//! ```
//! or if you want to time it and skip the overhead of checking if the current binary is up to date:
//! ```text
//! ~> time ./target/release/fibo_4784969 > fibo.out
//!
//! real    0m0.390s
//! user    0m0.386s
//! sys     0m0.004s
//!
//! ~> echo `head -c 32 fibo.out`
//! 10727395641800477229364813596225
//! ~> tail -c 33 fibo.out
//! 74856539211500699706378405156269
//!
//! ```

use ibig::{ubig, UBig};

/// Compute F<sub>k</ub> and F<sub>k+1</sub>
///
/// Recursively compute the Fibonacci numbers F<sub>k</sub> and F<sub>k+1</sub>, using the
/// recurrence relations
/// * (F<sub>2k</sub>, F<sub>2k+1</sub>)
///   = (F<sub>k</sub>[2F<sub>k+1</sub>-F<sub>k</sub>], F<sub>k+1</sub>(2F<sub>k+1</sub>-F<sub>k</sub>] + (-1)<sup>k+1</sup>)
/// * (F<sub>2k+1</sub>, F<sub>2k+2</sub>)
///   = (F<sub>k</sub>[2F<sub>k</sub>+F<sub>k+1</sub>] + (-1)<sup>k</sup>, F<sub>k+1</sub>[2F<sub>k</sub>+F<sub>k+1</sub>])
fn fibonacci_work(k: u32) -> (UBig, UBig)
{
    let one = ubig!(1);

    if k == 0
    {
        (ubig!(0), one)
    }
    else
    {
        let (a, b) = fibonacci_work(k / 2);
        if k % 2 == 0
        {
            let t = (&b << 1) - &a;
            if k % 4 == 0 { (&a*&t, &b*&t - one) } else { (&a*&t, &b*&t + one) }
        }
        else
        {
            let t = (&a << 1) + &b;
            if k % 4 == 1 { (&a*&t + one, &b*&t) } else { (&a*&t - one, &b*&t) }
        }
    }
}

/// Compute a Fibonacci number
///
/// Compute the `k`'th Fibonacci number F<sub>k</sub>
fn fibonacci(k: u32) -> UBig
{
    if k < 2
    {
        ubig!(k)
    }
    else
    {
        let (a, b) = fibonacci_work((k-1) / 2);
        if k % 2 == 0
        {
            ((&a << 1) + &b) * &b
        }
        else
        {
            let one = ubig!(1);
            let t = ((&b << 1) - &a) * &b;
            if k % 4 == 1 { &t - one } else { &t + one }
        }
    }
}

fn main()
{
    let res = fibonacci(4784969);
    println!("{}", res);
}
