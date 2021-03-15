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
//! This program can use a number of different backends:
//! * The excellent [ibig](https://crates.io/crates/ibig) crate written by Tomek Czajka. This crate is
//!   written in pure Rust, and is much faster than `num_bigint`. This option is the default;
//! * The standard goto backend for handling big integers in Rust: [num-bigint](https://crates.io/crates/num-bigint).
//!   Although this crate is certainly able to *compute* the required number in a few tenths of seconds,
//!   converting the result to decimal form for printing takes a whopping 21 seconds on a
//!   Core i7-6700HQ laptop. The `num-bigint` formatting implementation can certainly be improved upon:
//!   a recursive divide-and-and-conquer string conversion function takes the required time down to ~2s.
//!   Even then `ibig` is much faster;
//! * Or cheat and use the [rust bindings](https://crates.io/crates/rust-gmp) for the well known
//!   Gnu MP library. This backend is of course written in C, with only bindings to Rust. It is however,
//!   by far the fastest, and sets a baseline for other soluations.
//!
//! # Compilation
//!
//! `cargo build --release`
//!
//! # Run
//! ```text
//! cargo run --release
//! ```
//! to run with the default (`ibig`) backend. Use
//! ```text
//! cargo run --release -- -h
//! ```
//! to see all options. If you want to time the program and skip the overhead of checking if the current
//! binary is up to date, run the executable directly:
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
//! ```
//! Use the `-b` (or `--backend`) flag to select a backend:
//! ```
//! ~> time ./target/release/fibo_4784969 -b gmp > fibo2.out
//!
//! real    0m0.122s
//! user    0m0.115s
//! sys     0m0.007s
//! ```

const NAME: &'static str = env!("CARGO_PKG_NAME");
const VERSION: &'static str = env!("CARGO_PKG_VERSION");
const AUTHOR: &'static str = env!("CARGO_PKG_AUTHORS");
const DESCRIPTION: &'static str = env!("CARGO_PKG_DESCRIPTION");

/// Compute F<sub>k</ub> and F<sub>k+1</sub>
///
/// Recursively compute the Fibonacci numbers F<sub>k</sub> and F<sub>k+1</sub>, using the
/// recurrence relations
/// * (F<sub>2k</sub>, F<sub>2k+1</sub>)
///   = (F<sub>k</sub>[2F<sub>k+1</sub>-F<sub>k</sub>], F<sub>k+1</sub>(2F<sub>k+1</sub>-F<sub>k</sub>] + (-1)<sup>k+1</sup>)
/// * (F<sub>2k+1</sub>, F<sub>2k+2</sub>)
///   = (F<sub>k</sub>[2F<sub>k</sub>+F<sub>k+1</sub>] + (-1)<sup>k</sup>, F<sub>k+1</sub>[2F<sub>k</sub>+F<sub>k+1</sub>])
fn fibonacci_work<T>(k: u32) -> (T, T)
where T: From<u32>
        + for <'a> std::ops::Add<&'a T, Output=T>
        + for <'a> std::ops::Sub<&'a T, Output=T>,
    for <'a> &'a T: std::ops::Mul<&'a T, Output=T>
        + std::ops::Shl<usize, Output=T>
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
            let t = (&b << 1) - &a;
            if k % 4 == 0 { (&a*&t, &b*&t - &one) } else { (&a*&t, &b*&t + &one) }
        }
        else
        {
            let t = (&a << 1) + &b;
            if k % 4 == 1 { (&a*&t + &one, &b*&t) } else { (&a*&t - &one, &b*&t) }
        }
    }
}

/// Compute a Fibonacci number
///
/// Compute the `k`'th Fibonacci number F<sub>k</sub>
fn fibonacci<T>(k: u32) -> T
where T: From<u32>
        + for <'a> std::ops::Add<&'a T, Output=T>
        + for <'a> std::ops::Mul<&'a T, Output=T>
        + for <'a> std::ops::Sub<&'a T, Output=T>,
    for <'a> &'a T: std::ops::Mul<&'a T, Output=T>
        + std::ops::Shl<usize, Output=T>
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
            ((&a << 1) + &b) * &b
        }
        else
        {
            let one = T::from(1);
            let t = ((&b << 1) - &a) * &b;
            if k % 4 == 1 { t - &one } else { t + &one }
        }
    }
}

fn main()
{
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
            .possible_values(&["ibig", "gmp", "num_bigint"])
            .default_value("ibig")
        )
        .get_matches();

    let k = 4784969u32;
    match matches.value_of("backend")
    {
        Some("ibig")       => { println!("{}", fibonacci::<ibig::UBig>(k)); }
        Some("gmp")        => { println!("{}", fibonacci::<gmp::mpz::Mpz>(k)); }
        Some("num_bigint") => { println!("{}", fibonacci::<num_bigint::BigUint>(k)); }
        _                  => { unreachable!(); }
    }
}
