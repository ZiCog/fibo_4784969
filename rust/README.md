# fibo_4784969

A program to compute the first one-million digits Fibonacci number

This program computes the first Fibonacci number consisting of one million decimal digits,
which happend to be F<sub>4784969</sub>. It is based upon the doubling formulae from the
FreeBasic version by Eric Olson:
* (F<sub>2k</sub>, F<sub>2k+1</sub>)
  = (F<sub>k</sub>[2F<sub>k+1</sub>-F<sub>k</sub>], F<sub>k+1</sub>(2F<sub>k+1</sub>-F<sub>k</sub>] + (-1)<sup>k+1</sup>)
* (F<sub>2k+1</sub>, F<sub>2k+2</sub>)
= (F<sub>k</sub>[2F<sub>k</sub>+F<sub>k+1</sub>] + (-1)<sup>k</sup>, F<sub>k+1</sub>[2F<sub>k</sub>+F<sub>k+1</sub>])

This program can use a number of different backends using the `-b BACKEND` command line option.
Possible values for `BACKEND` are:
* `binary`: The bundled minimal big integer implementation, using binary digits. This computes
  the number faster than the `decimal` backend, but printing the result in decimal form is
  currently horrendously slow;
* `decimal`: The bundled minimal big integer implementation, using decimal digits. This is
  slower in computation than the `binary` backend, but printing the result in decimal form is
  much faster as the digits are already decimal. This option is the default;
* `ibig`: The excellent [ibig](https://crates.io/crates/ibig) crate written by Tomek Czajka.
  This crate is written in pure Rust, and is much faster than `num_bigint`.
* `num_bigint`: The standard goto backend for handling big integers in Rust:
  [num-bigint](https://crates.io/crates/num-bigint). Although this crate is certainly able to
  *compute* the required number in a few tenths of seconds, it suffers from the same problem
  as the `binary` backend: converting the result to decimal form for printing takes a whopping
  21 seconds on a Core i7-6700HQ laptop. The `num-bigint` formatting implementation can
  certainly be improved upon: a recursive divide-and-and-conquer string conversion function
  takes the required time down to ~2s. Even then `ibig` is much faster;
* `gmp`: Cheat and use the [rust bindings](https://crates.io/crates/rust-gmp) for the well known
  Gnu MP library. This backend is of course written in C, with only bindings to Rust. It is however,
  by far the fastest, and sets a baseline for other solutions;
* `rug`: The [rug](https://crates.io/crates/rug) crate, which acts as a higher level frontend to
  several numerical libraries, in this case again for GMP. As both the `rug` and the `gmp`
  backend use the GMP library for their calculations, their performance is similar.

# Compilation

`cargo build --release`

# Run
```text
cargo run --release
```
to run with the default (`decimal`) backend. Use
```text
cargo run --release -- -h
```
to see all options. If you want to time the program and skip the overhead of checking if the current
binary is up to date, run the executable directly:
```text
~> ./target/release/fibo_4784969 > fibo.out
computing F(4784969): 0.962s
printing F(4784969): 0.008s
~> echo `head -c 32 fibo.out`
10727395641800477229364813596225
~> tail -c 33 fibo.out
74856539211500699706378405156269
```
Use the `-b` (or `--backend`) flag to select a backend:
```
~> ./target/release/fibo_4784969 -b gmp > fibo2.out
computing F(4784969): 0.048s
printing F(4784969): 0.090s
```
Or try them all:
```
~> for backend in binary decimal ibig gmp num_bigint rug; do
    echo "Backend $backend:"
    ./target/release/fibo_4784969 -b $backend > fibo_$backend.out
done
Backend binary:
computing F(4784969): 0.351s
printing F(4784969): 22.800s
Backend decimal:
computing F(4784969): 0.938s
printing F(4784969): 0.008s
Backend ibig:
computing F(4784969): 0.108s
printing F(4784969): 0.324s
Backend gmp:
computing F(4784969): 0.021s
printing F(4784969): 0.093s
Backend num_bigint:
computing F(4784969): 0.137s
printing F(4784969): 23.409s
Backend rug:
computing F(4784969): 0.024s
printing F(4784969): 0.091s
```
