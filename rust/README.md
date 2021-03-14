# fibo_4784969

A program to compute the first one-million digits Fivonacci number

This program computes the first Fibonacci number consisting of one million decimal digits,
which happend to be F<sub>4784969</sub>. It is based upon the doubling formulae from the
FreeBasic version by Eric Olson:
* (F<sub>2k</sub>, F<sub>2k+1</sub>)
  = (F<sub>k</sub>[2F<sub>k+1</sub>-F<sub>k</sub>], F<sub>k+1</sub>(2F<sub>k+1</sub>-F<sub>k</sub>] + (-1)<sup>k+1</sup>)
* (F<sub>2k+1</sub>, F<sub>2k+2</sub>)
= (F<sub>k</sub>[2F<sub>k</sub>+F<sub>k+1</sub>] + (-1)<sup>k</sup>, F<sub>k+1</sub>[2F<sub>k</sub>+F<sub>k+1</sub>])

This program utilises the extremely fast [ibig](https://crates.io/crates/ibig) library written
by Tomek Czajka. The canonical solution for big integer arithmetic would probably be
[num-bigint](https://crates.io/crates/num-bigint), but while that crate is certainly able to
*compute* the required number in a few tenths of seconds, converting the result to decimal form
for printing takes a whopping 21 seconds on a Core(TM) i7-6700HQ laptop. The `num-bigint`
formatting implementation can certainly be improved upon: a recursive divide-and-and-conquer
string conversion function takes the required time down to ~2s. Even then `ibig` is much faster,
without having to write our own formatting routines.

# Compilation

`cargo build --release`

# Run

```text
cargo run --release
```
or if you want to time it and skip the overhead of checking if the current binary is up to date:
```text
~> time ./target/release/fibo_4784969 > fibo.out

real    0m0.390s
user    0m0.386s
sys     0m0.004s

~> echo `head -c 32 fibo.out`
10727395641800477229364813596225
~> tail -c 33 fibo.out
74856539211500699706378405156269

```
