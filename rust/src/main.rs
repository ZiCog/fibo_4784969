//! A program to compute the first one-million digits Fibonacci number
//!
//! This program computes the first Fibonacci number consisting of one million decimal digits,
//! which happend to be F<sub>4784969</sub>. It is based upon the doubling formulae from the
//! FreeBasic version by Eric Olson:
//! * (F<sub>2k</sub>, F<sub>2k+1</sub>)
//!   = (F<sub>k</sub>[2F<sub>k+1</sub>-F<sub>k</sub>], F<sub>k+1</sub>(2F<sub>k+1</sub>-F<sub>k</sub>] + (-1)<sup>k+1</sup>)
//! * (F<sub>2k+1</sub>, F<sub>2k+2</sub>)
//! = (F<sub>k</sub>[2F<sub>k</sub>+F<sub>k+1</sub>] + (-1)<sup>k</sup>, F<sub>k+1</sub>[2F<sub>k</sub>+F<sub>k+1</sub>])
//!
//! This program can use a number of different backends using the `-b BACKEND` command line option.
//! Possible values for `BACKEND` are:
//! * `binary`: The bundled minimal big integer implementation, using binary digits. This computes
//!   the number faster than the `decimal` backend, but printing the result in decimal form is
//!   currently horrendously slow;
//! * `decimal`: The bundled minimal big integer implementation, using decimal digits. This is
//!   slower in computation than the `binary` backend, but printing the result in decimal form is
//!   much faster as the digits are already decimal. This option is the default;
//! * `ibig`: The excellent [ibig](https://crates.io/crates/ibig) crate written by Tomek Czajka.
//!   This crate is written in pure Rust, and is much faster than `num_bigint`.
//! * `num_bigint`: The standard goto backend for handling big integers in Rust:
//!   [num-bigint](https://crates.io/crates/num-bigint). Although this crate is certainly able to
//!   *compute* the required number in a few tenths of seconds, it suffers from the same problem
//!   as the `binary` backend: converting the result to decimal form for printing takes a whopping
//!   21 seconds on a Core i7-6700HQ laptop. The `num-bigint` formatting implementation can
//!   certainly be improved upon: a recursive divide-and-and-conquer string conversion function
//!   takes the required time down to ~2s. Even then `ibig` is much faster;
//! * `gmp`: Cheat and use the [rust bindings](https://crates.io/crates/rust-gmp) for the well known
//!   Gnu MP library. This backend is of course written in C, with only bindings to Rust. It is however,
//!   by far the fastest, and sets a baseline for other solutions;
//! * `rug`: The [rug](https://crates.io/crates/rug) crate, which acts as a higher level frontend to
//!   several numerical libraries, in this case again for GMP. As both the `rug` and the `gmp`
//!   backend use the GMP library for their calculations, their performance is similar.
//!
//! # Compilation
//!
//! `cargo build --release`
//!
//! # Run
//! ```text
//! cargo run --release
//! ```
//! to run with the default (`decimal`) backend. Use
//! ```text
//! cargo run --release -- -h
//! ```
//! to see all options. If you want to time the program and skip the overhead of checking if the current
//! binary is up to date, run the executable directly:
//! ```text
//! ~> ./target/release/fibo_4784969 > fibo.out
//! computing F(4784969): 0.110s
//! printing F(4784969): 0.328s
//! ~> echo `head -c 32 fibo.out`
//! 10727395641800477229364813596225
//! ~> tail -c 33 fibo.out
//! 74856539211500699706378405156269
//! ```
//! Use the `-b` (or `--backend`) flag to select a backend:
//! ```
//! ~> ./target/release/fibo_4784969 -b gmp > fibo2.out
//! computing F(4784969): 0.048s
//! printing F(4784969): 0.090s
//! ```
//! Or try them all:
//! ```
//! ~> for backend in binary decimal ibig gmp num_bigint rug; do
//!     echo "Backend $backend:"
//!     ./target/release/fibo_4784969 -b $backend > fibo_$backend.out
//! done
//! Backend binary:
//! computing F(4784969): 0.351s
//! printing F(4784969): 22.800s
//! Backend decimal:
//! computing F(4784969): 0.938s
//! printing F(4784969): 0.008s
//! Backend ibig:
//! computing F(4784969): 0.108s
//! printing F(4784969): 0.324s
//! Backend gmp:
//! computing F(4784969): 0.021s
//! printing F(4784969): 0.093s
//! Backend num_bigint:
//! computing F(4784969): 0.137s
//! printing F(4784969): 23.409s
//! Backend rug:
//! computing F(4784969): 0.024s
//! printing F(4784969): 0.091s
//! ```

// Clippy does not like Allman indentation.
#![allow(clippy::suspicious_else_formatting)]

mod decimal;
mod digit;
mod myubig;

use clap::value_t;

const NAME: &str = env!("CARGO_PKG_NAME");
const VERSION: &str = env!("CARGO_PKG_VERSION");
const AUTHOR: &str = env!("CARGO_PKG_AUTHORS");
const DESCRIPTION: &str = env!("CARGO_PKG_DESCRIPTION");

/// Common trait for the operations we require to compute a Fibonacci number
trait Number: From<u32>
        + for <'a> std::ops::Add<&'a Self, Output=Self>
        + for <'a> std::ops::Mul<&'a Self, Output=Self>
        + for <'a> std::ops::Sub<&'a Self, Output=Self>
{
    /// The various big integer implementations can't seem to agree on what types to implement
    /// a left shift for. We only use it for doubling a value, so provide a trait function to
    /// do the doubling. We could possibly use addition for the same effect without having to
    /// implement `double()` for all supported types, but that would most likely be slower.
    fn double(&self) -> Self;
}

type Dec32 = decimal::DecimalDigit<u32>;

impl<T> Number for myubig::UBig<T>
where T: crate::digit::Digit, myubig::UBig<T>: From<u32>
{
    fn double(&self) -> Self
    {
        let mut res = self.clone();
        myubig::UBig::<T>::double(&mut res);
        res
    }
}

impl Number for ibig::UBig
{
    fn double(&self) -> Self
    {
        self << 1u8
    }
}

impl Number for gmp::mpz::Mpz
{
    fn double(&self) -> Self
    {
        self << 1usize
    }
}

impl Number for num_bigint::BigUint
{
    fn double(&self) -> Self
    {
        self << 1u8
    }
}

impl Number for rug::Integer
{
    fn double(&self) -> Self
    {
        self.clone() << 1u32
    }
}

/// Compute F<sub>k</ub> and F<sub>k+1</sub>
///
/// Recursively compute the Fibonacci numbers F<sub>k</sub> and F<sub>k+1</sub>, using the
/// recurrence relations
/// * (F<sub>2k</sub>, F<sub>2k+1</sub>)
///   = (F<sub>k</sub>[2F<sub>k+1</sub>-F<sub>k</sub>], F<sub>k+1</sub>(2F<sub>k+1</sub>-F<sub>k</sub>] + (-1)<sup>k+1</sup>)
/// * (F<sub>2k+1</sub>, F<sub>2k+2</sub>)
///   = (F<sub>k</sub>[2F<sub>k</sub>+F<sub>k+1</sub>] + (-1)<sup>k</sup>, F<sub>k+1</sub>[2F<sub>k</sub>+F<sub>k+1</sub>])
fn fibonacci_work<T>(k: u32) -> (T, T)
where T: Number
{
    let one = T::from(1);

    if k == 0
    {
        (T::from(0), one)
    }
    else
    {
        let (a, b) = fibonacci_work::<T>(k / 2);
        if k % 2 == 0
        {
            let t = b.double() - &a;
            if k % 4 == 0 { (a*&t, b*&t - &one) } else { (a*&t, b*&t + &one) }
        }
        else
        {
            let t = a.double() + &b;
            if k % 4 == 1 { (a*&t + &one, b*&t) } else { (a*&t - &one, b*&t) }
        }
    }
}

/// Compute a Fibonacci number
///
/// Compute the `k`'th Fibonacci number F<sub>k</sub>
fn fibonacci<T>(k: u32) -> T
where T: Number
{
    if k < 2
    {
        T::from(k)
    }
    else
    {
        let (a, b) = fibonacci_work::<T>((k-1) / 2);
        if k % 2 == 0
        {
            (a.double() + &b) * &b
        }
        else
        {
            let one = T::from(1);
            let t = (b.double() - &a) * &b;
            if k % 4 == 1 { t - &one } else { t + &one }
        }
    }
}

fn test_fibonacci<T>(k: u32)
where T: Number + std::fmt::Display
{
    let tic = std::time::Instant::now();
    let fk = fibonacci::<T>(k);
    let toc = std::time::Instant::now();
    let duration = toc.duration_since(tic);
    eprintln!("computing F({}): {}.{:03}s", k, duration.as_secs(), duration.subsec_millis());

    let tic = std::time::Instant::now();
    println!("{}", fk);
    let toc = std::time::Instant::now();
    let duration = toc.duration_since(tic);
    eprintln!("printing F({}): {}.{:03}s", k, duration.as_secs(), duration.subsec_millis());
}

fn main()
{
    let default_k = 4784969u32;
    let default_k_str = default_k.to_string();

    let matches = clap::App::new(NAME)
        .version(VERSION)
        .author(AUTHOR)
        .about(DESCRIPTION)
        .arg(clap::Arg::with_name("backend")
            .short("b")
            .long("backend")
            .value_name("BACKEND")
            .help("Choose which backend to use for the computation")
            .takes_value(true)
            .possible_values(&["binary", "decimal", "ibig", "gmp", "num_bigint", "rug"])
            .default_value("decimal")
        )
        .arg(clap::Arg::with_name("number")
            .short("n")
            .long("number")
            .value_name("N")
            .help("Compute the N'th Fibonacci number")
            .takes_value(true)
            .default_value(&default_k_str)
         )
        .get_matches();

    let k = value_t!(matches, "number", u32).unwrap_or_else(|e| e.exit());
    match matches.value_of("backend")
    {
        Some("binary")     => { test_fibonacci::<myubig::UBig<u32>>(k); },
        Some("decimal")    => { test_fibonacci::<myubig::UBig<Dec32>>(k); },
        Some("ibig")       => { test_fibonacci::<ibig::UBig>(k); },
        Some("gmp")        => { test_fibonacci::<gmp::mpz::Mpz>(k); },
        Some("num_bigint") => { test_fibonacci::<num_bigint::BigUint>(k); },
        Some("rug")        => { test_fibonacci::<rug::Integer>(k); }
        _                  => { unreachable!(); }
    }
}
