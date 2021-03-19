use crate::decimal::DecimalDigit;
use crate::digit::Digit;
use num_traits::{Zero, One};

/// Error type for big integer operations
#[derive(Debug, PartialEq)]
pub enum Error
{
    /// Attempt to subtract a larger number from a smaller one
    Underflow
}

/// Structure used for implementing arithmetic operations on an immutable borrow of big integer digits
struct UBigBorrow<'a, T>
{
    /// The digits borrowed
    digits: &'a [T]
}

impl<'a, T: 'a> UBigBorrow<'a, T>
where T: Digit
{
    /// Cutoff length for switching from long multiplication to Karatsuba multiplication.
    /// NOTE: Should be at least 4.
    const KARATSUBA_CUTOFF: usize = 16;

    /// Create a new borrow of digits `digits`
    fn new(digits: &'a [T]) -> Self
    {
        UBigBorrow { digits }
    }

    /// Split this borrow into two new borrows before digit `pos`
    fn split_at(&self, pos: usize) -> (Self, Self)
    {
        let (low, high) = self.digits.split_at(pos);
        (Self::new(low), Self::new(high))
    }

    /// Check if this borrow is empty. This is a valid digit range, and represents zero.
    fn is_empty(&self) -> bool
    {
        self.digits.is_empty()
    }

    /// Return the number of digits in this borrow
    fn nr_digits(&self) -> usize
    {
        self.digits.len()
    }

    /// Add these digits to those in `other`, and store the result in `result`. The output
    /// array must be large enough to hold the result including a possible carry, i.e. must
    /// have a length of at least `max(n0, n1) + 1`, where `n0` denotes the number of digits in
    /// this borrow, and `n1` the number of digits in `other`. Returns the number of digits
    /// in the resulting sum.
    fn add_into(&self, other: &Self, result: &mut [T]) -> usize
    {
        let n0 = self.nr_digits();
        let n1 = other.nr_digits();
        let nmin = n0.min(n1);
        let nmax = n0.max(n1);
        assert!(result.len() >= nmax + 1);

        let mut carry = T::LongDigitType::zero();
        for ((d0, d1), dr) in self.digits.iter().zip(other.digits).zip(result.iter_mut())
        {
            let tmp = d0.to_long() + d1.to_long() + carry;
            *dr = T::to_short(tmp % T::RADIX);
            carry = tmp / T::RADIX;
        }

        match n0.cmp(&n1)
        {
            std::cmp::Ordering::Equal   => { /* do nothing */ },
            std::cmp::Ordering::Less    => { result[nmin..nmax].copy_from_slice(&other.digits[nmin..]); },
            std::cmp::Ordering::Greater => { result[nmin..nmax].copy_from_slice(&self.digits[nmin..]); }
        }

        result[nmax] = T::zero();
        if !carry.is_zero()
        {
            UBigMutBorrow::new(&mut result[nmin..nmax+1]).inc();
        }

        let mut n = nmax + 1;
        while n > 0 && result[n-1].is_zero()
        {
            n -= 1;
        }

        n
    }

    /// Calculate the maximum size of the scratch array necessary to perform Karatsuba
    /// multiplication on two `n`-digit numbers.
    fn calc_karatsuba_work_size(n: usize) -> usize
    {
        let mut work_size = 0;
        let mut nn = n;
        while nn >= Self::KARATSUBA_CUTOFF
        {
            let split1 = (nn + 1) / 2 + 1;
            work_size += 2 * split1;
            nn = split1;
        }
        work_size
    }

    /// Multiply the number represented by these digits by `other`, and store the result in
    /// `result`. The result array must have space for at least `n0+n1` digits, where `n0`
    /// denotes the number of digits in ths borrow, and `n1` the number of digits in `other`.
    /// Returns the number of digits in the product.
    fn multiply_into(&self, other: &Self, result: &mut [T]) -> usize
    {
        let n0 = self.nr_digits();
        let n1 = other.nr_digits();
        assert!(result.len() >= n0 + n1, "Not enough space to store the result");

        if self.is_empty() || other.is_empty()
        {
            0
        }
        else if n0 >= Self::KARATSUBA_CUTOFF && n1 >= Self::KARATSUBA_CUTOFF
        {
            let work_size = Self::calc_karatsuba_work_size(self.nr_digits().max(other.nr_digits()));
            let mut work = vec![T::zero(); work_size];
            self.multiply_karatsuba_into(other, result, &mut work)
        }
        else
        {
            self.multiply_long_into(other, result)
        }
    }

    /// Multiply the number represented by these digits by `other`, and store the result in
    /// `result`. When the numbers are large enough for Karatsuba multiplication, the scratch
    /// array `work` is used, which should be of appropriate size (see
    /// [calc_karatsuba_work_size()](Self::calc_karatsuba_work_size)).
    /// The result array must have space for at least `n0+n1` digits, where `n0`
    /// denotes the number of digits in ths borrow, and `n1` the number of digits in `other`.
    /// Returns the number of digits in the product.
    fn multiply_into_work(&self, other: &Self, result: &mut [T], work: &mut [T]) -> usize
    {
        let n0 = self.nr_digits();
        let n1 = other.nr_digits();
        assert!(result.len() >= n0 + n1, "Not enough space to store the result");

        if self.is_empty() || other.is_empty()
        {
            0
        }
        else if n0 >= Self::KARATSUBA_CUTOFF && n1 >= Self::KARATSUBA_CUTOFF
        {
            self.multiply_karatsuba_into(other, result, work)
        }
        else
        {
            self.multiply_long_into(other, result)
        }
    }

    /// Multiply the number represented by these digits by `other` using long multiplication, and
    /// store the result in `result`. The result array must have space for at least `n0+n1` digits,
    /// where `n0` denotes the number of digits in ths borrow, and `n1` the number of digits in
    /// `other`. Returns the number of digits in the product.
    fn multiply_long_into(&self, other: &Self, result: &mut [T]) -> usize
    {
        let n0 = self.nr_digits();
        let n1 = other.nr_digits();
        let mut n = n0 + n1;

        result[..n].fill(T::zero());
        for (offset, &digit1) in other.digits.iter().enumerate()
        {
            let ldigit1 = digit1.to_long();
            let mut carry = T::LongDigitType::zero();
            for (&digit0, rdigit) in self.digits.iter().zip(&mut result[offset..])
            {
                carry += digit0.to_long() * ldigit1 + rdigit.to_long();
                *rdigit = T::to_short(carry % T::RADIX);
                carry /= T::RADIX;
            }
            result[offset+n0] = T::to_short(carry);
        }

        while n > 0  && result[n-1].is_zero()
        {
            n -= 1;
        }
        n
    }

    /// Multiply the number represented by these digits by `other` using Karatsuba multiplication,
    /// and store the result in `result`. The scratch should be of appropriate size
    /// (see [calc_karatsuba_work_size()](Self::calc_karatsuba_work_size)). The result array must
    /// have space for at least `n0+n1` digits, where `n0` denotes the number of digits in ths
    /// borrow, and `n1` the number of digits in `other`. Returns the number of digits in the product.
    fn multiply_karatsuba_into(&self, other: &Self, result: &mut [T], work: &mut [T]) -> usize
    {
        let n0 = self.nr_digits();
        let n1 = other.nr_digits();
        let nmax = n0.max(n1);
        assert!(n0 >= 4 && n1 >= 4, "Number of digits should be at least 4 for Karatsuba multiplication");
        assert!(work.len() >= Self::calc_karatsuba_work_size(nmax), "Insufficient work space");

        let split = (nmax + 1) / 2;

        let (low0, high0) = self.split_at(split.min(n0));
        let (low1, high1) = other.split_at(split.min(n1));

        let (sum0, sum1) = result.split_at_mut(split+1);
        let nsum0 = low0.add_into(&high0, sum0);                    // low0 + high0
        let nsum1 = low1.add_into(&high1, sum1);                    // low1 + high1

        let (z1, new_work) = work.split_at_mut(2*split+2);
        let mut nz1 = UBigBorrow::new(&sum0[..nsum0])               // (low0+high0)*(low1+high1)
            .multiply_into_work(&UBigBorrow::new(&sum1[..nsum1]), z1, new_work);
        result[..n0+n1].fill(T::zero());
        let (z0, z2) = result.split_at_mut(2*split);
        let nz0 = low0.multiply_into_work(&low1, z0, new_work);     // low0*low1
        let nz2 = high0.multiply_into_work(&high1, z2, new_work);   // high0*high1

        let mut bz1 = UBigMutBorrow::new(&mut z1[..nz1]);
        bz1.sub(&UBigBorrow::new(&z2[..nz2]));
        bz1.sub(&UBigBorrow::new(&z0[..nz0]));                      // low0*high1 + high0*low1
        while z1[nz1-1].is_zero()
        {
            nz1 -= 1;
        }

        let carry = UBigMutBorrow::new(&mut result[split..]).add(&UBigBorrow::new(&z1[..nz1]));
        assert!(carry.is_none());
        let mut n = n0 + n1;
        while n > 0 && result[n-1].is_zero()
        {
            n -= 1;
        }
        n
    }
}

/// Structure used for implementing arithmetic operations on a mutable borrow of big integer digits
struct UBigMutBorrow<'a, T>
{
    digits: &'a mut [T]
}

impl<'a, T> UBigMutBorrow<'a, T>
where T: Digit
{
    /// Create a new mutable borrow of digits `digits`
    fn new(digits: &'a mut [T]) -> Self
    {
        UBigMutBorrow { digits }
    }

    /// Return the number of digits borrowed
    fn nr_digits(&self) -> usize
    {
        self.digits.len()
    }

    /// Return a new mutable borrow of part of these digits, starting from position `start` and
    /// continuing to the end of the array.
    fn part_from(&mut self, start: usize) -> UBigMutBorrow<T>
    {
        UBigMutBorrow::new(&mut self.digits[start..])
    }

    /// Increment the number represented by this borrow by one. Returns the carry digit if the
    /// increment overflows, `None` otherwise.
    fn inc(&mut self) -> Option<T>
    {
        let one = T::one();
        for d in self.digits.iter_mut()
        {
            if *d < T::MAX
            {
                *d += one;
                return None;
            }
            else
            {
                *d = T::zero();
            }
        }
        Some(one)
    }

    /// Decrement the number represented by this borrow by one. Returns the carry (borrow) digit
    /// if the decrement underflows, `None` otherwise.
    fn dec(&mut self) -> Option<T>
    {
        let one = T::one();
        for d in self.digits.iter_mut()
        {
            if !d.is_zero()
            {
                *d -= one;
                return None;
            }
            else
            {
                *d = T::MAX;
            }
        }
        Some(one)
    }

    /// Double the number represented by this borrow. Returns the carry digit if the doubling
    /// overflows the array, `None` otherwise.
    fn double(&mut self) -> Option<T>
    {
        let mut carry = T::LongDigitType::zero();
        for d in self.digits.iter_mut()
        {
            let tmp = (d.to_long() << 1) + carry;
            *d = T::to_short(tmp % T::RADIX);
            carry = tmp / T::RADIX;
        }
        (!carry.is_zero()).then(|| T::to_short(carry))
    }

    /// Add the number represented by `borrow` to this. The number of digits in `other` should
    /// not be greater than the number of digits in `self`. Returns the carry digit if the addition
    /// overflows the array, `None` otherwise.
    fn add(&mut self, other: &UBigBorrow<T>) -> Option<T>
    {
        let n1 = other.nr_digits();
        assert!(n1 <= self.nr_digits());

        let mut carry = T::LongDigitType::zero();
        for (d0, &d1) in self.digits.iter_mut().zip(other.digits)
        {
            let tmp = d0.to_long() + d1.to_long() + carry;
            *d0 = T::to_short(tmp % T::RADIX);
            carry = tmp / T::RADIX;
        }

        if !carry.is_zero() { self.part_from(n1).inc() } else { None }
    }

    /// Subtract the number represented by `other` from this. The number of digits in `other`
    /// should not be greater than the number of digits in `self`. Returns the carry (borrow) digit
    /// on underflow, `None` otherwise.
    ///
    /// NOTE: this does not drop leading zeros after the subtraction (which would be impossible,
    /// as there is no notion of the number of digits used here). If removing leading zeros is
    /// desired, the caller is responsible for doing so.
    fn sub(&mut self, other: &UBigBorrow<T>) -> Option<T>
    {
        let n1 = other.nr_digits();
        assert!(n1 <= self.nr_digits());

        let mut carry = T::LongDigitType::zero();
        for (d0, &d1) in self.digits.iter_mut().zip(other.digits)
        {
            let tmp = T::RADIX + d0.to_long() - d1.to_long() - carry;
            *d0 = T::to_short(tmp % T::RADIX);
            carry = T::LongDigitType::one() - tmp / T::RADIX;
        }

        if !carry.is_zero() { self.part_from(n1).dec() } else { None }
    }
}

/// Structure describing an unsigned big integer
#[derive(Clone, Debug, Eq, PartialEq)]
pub struct UBig<T>
{
    /// The digits making up the number
    digits: Vec<T>
}

impl<T> UBig<T>
where T: Digit
{
    /// Create a new big integer from the digits in `digits`
    pub fn new(digits: Vec<T>) -> Self
    {
        let mut num = UBig { digits };
        num.drop_leading_zeros();
        num
    }

    /// Return the number of digits in this number
    pub fn nr_digits(&self) -> usize
    {
        self.digits.len()
    }

    /// Return an immutable borrow of the digit array in this number
    pub fn digits(&self) -> &[T]
    {
        &self.digits
    }

    /// Return an immutable borrow of the digits of this number
    fn borrowed(&self) -> UBigBorrow<T>
    {
        UBigBorrow::new(&self.digits)
    }

    /// Return an immutable borrow of the first `end` digits of this number
    fn borrowed_upto(&self, end: usize) -> UBigBorrow<T>
    {
        UBigBorrow::new(&self.digits[..end])
    }

    /// Return a mutable borrow of digits of this number
    fn borrowed_mut(&mut self) -> UBigMutBorrow<T>
    {
        UBigMutBorrow::new(&mut self.digits)
    }

    /// Return a mutable borrow of digits of this number from position `start` onwards
    fn borrowed_mut_from(&mut self, start: usize) -> UBigMutBorrow<T>
    {
        UBigMutBorrow::new(&mut self.digits[start..])
    }

    /// Increment this number by one
    pub fn inc(&mut self) -> &mut Self
    {
        if let Some(digit) = self.borrowed_mut().inc()
        {
            self.digits.push(digit);
        }
        self
    }

    /// Decrement this number by one
    pub fn dec(&mut self) -> Result<&mut Self, Error>
    {
        if self.borrowed_mut().dec().is_some()
        {
            Err(Error::Underflow)
        }
        else
        {
            Ok(self.drop_leading_zeros())
        }
    }

    /// Double this number
    pub fn double(&mut self) -> &mut Self
    {
        if let Some(digit) = self.borrowed_mut().double()
        {
            self.digits.push(digit);
        }
        self
    }

    /// Drop any leading zeros from the digits array
    fn drop_leading_zeros(&mut self) -> &mut Self
    {
        while let Some(d) = self.digits.last()
        {
            if d.is_zero()
            {
                self.digits.pop();
            }
            else
            {
                break;
            }
        }
        self
    }

    /// Add `other` to this number
    pub fn add_assign_big(&mut self, other: &Self) -> &mut Self
    {
        self.add_assign_at_offset(other, 0)
    }

    /// Add `other * r<sup>offset</sup>` to this number, where `r` is the radix of this number.
    fn add_assign_at_offset(&mut self, other: &Self, offset: usize) -> &mut Self
    {
        let n0 = self.nr_digits();
        if n0 <= offset
        {
            self.digits.extend(std::iter::repeat(T::zero()).take(offset - n0));
            self.digits.extend(&other.digits);
        }
        else
        {
            let n1 = other.nr_digits();
            self.digits.extend(&other.digits[n1..]);
            if let Some(carry) = self.borrowed_mut_from(offset).add(&other.borrowed_upto(n1))
            {
                self.digits.push(carry);
            }
        }

        self
    }

    /// Subtract `other` from this number. Returns an `Underflow` error when `other` is larger
    /// than `self`.
    pub fn sub_assign_big(&mut self, other: &Self) -> Result<&mut Self, Error>
    {
        self.sub_assign_at_offset(other, 0)
    }

    /// Subtract `other * r<sup>offset</sup>` from this number, where `r` is the radix of this
    /// number. Returns an `Underflow` error when the number to be subtracted is larger than
    /// `self`.
    fn sub_assign_at_offset(&mut self, other: &Self, offset: usize) -> Result<&mut Self, Error>
    {
        let n0 = self.nr_digits();
        let n1 = other.nr_digits();
        if n0 < n1 + offset
            || self.borrowed_mut_from(offset).sub(&other.borrowed()).is_some()
        {
            Err(Error::Underflow)
        }
        else
        {
            Ok(self.drop_leading_zeros())
        }
    }

    /// Multiply this number by `other` and return the result
    pub fn mul_big(&self, other: &Self) -> Self
    {
        if self.is_zero() || other.is_zero()
        {
            Self::zero()
        }
        else
        {
            let n0 = self.nr_digits();
            let n1 = other.nr_digits();
            let mut digits = vec![T::zero(); n0+n1];
            let n = self.borrowed().multiply_into(&other.borrowed(), &mut digits);
            digits.truncate(n);
            UBig { digits }
        }
    }

    /// Raise this number to the power `exp`, and return the result
    pub fn pow(&self, exp: u32) -> Self
    {
        let mut power = self.clone();
        let mut result = Self::one();
        let mut n = exp;
        while n > 0
        {
            if n % 2 != 0
            {
                result *= &power;
            }
            n >>= 1;
            power = &power * &power;
        }
        result
    }
}

impl<T> num_traits::Zero for UBig<T>
where T: Digit
{
    fn zero() -> Self
    {
        UBig { digits: vec![] }
    }

    fn is_zero(&self) -> bool
    {
        self.digits.is_empty()
    }
}

impl<T> num_traits::One for UBig<T>
where T: Digit
{
    fn one() -> Self
    {
        UBig { digits: vec![T::one()] }
    }
}

impl<T> From<T> for UBig<T>
where T: Digit
{
    fn from(n: T) -> Self
    {
        if n.is_zero()
        {
            UBig::zero()
        }
        else
        {
            UBig { digits: vec![n] }
        }
    }
}

impl<T> From<T> for UBig<DecimalDigit<T>>
where DecimalDigit<T>: Digit
{
    fn from(n: T) -> Self
    {
        let d = DecimalDigit(n);
        if d.is_zero()
        {
            UBig::zero()
        }
        else
        {
            let d0 = DecimalDigit::to_short(d.to_long() % DecimalDigit::RADIX);
            let d1 = DecimalDigit::to_short(d.to_long() / DecimalDigit::RADIX);
            let mut big = UBig { digits: vec![d0, d1] };
            big.drop_leading_zeros();
            big
        }
    }
}

impl<T> PartialOrd for UBig<T>
where T: Digit + Ord
{
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering>
    {
        Some(self.cmp(other))
    }
}

impl<T> Ord for UBig<T>
where T: Digit + Ord
{
    fn cmp(&self, other: &Self) -> std::cmp::Ordering
    {
        self.nr_digits().cmp(&other.nr_digits())
            .then_with(|| self.digits().iter().rev().cmp(other.digits.iter().rev()))
    }
}

impl<T> std::ops::AddAssign<UBig<T>> for UBig<T>
where T: Digit
{
    fn add_assign(&mut self, other: Self)
    {
        *self += &other;
    }
}

impl<T> std::ops::AddAssign<&UBig<T>> for UBig<T>
where T: Digit
{
    fn add_assign(&mut self, other: &UBig<T>)
    {
        self.add_assign_big(other);
    }
}

impl<T> std::ops::SubAssign<UBig<T>> for UBig<T>
where T: Digit
{
    fn sub_assign(&mut self, other: Self)
    {
        *self -= &other;
    }
}

impl<T> std::ops::SubAssign<&UBig<T>> for UBig<T>
where T: Digit
{
    fn sub_assign(&mut self, other: &UBig<T>)
    {
        self.sub_assign_big(other).expect("Underflow while subtracting UBig");
    }
}

impl<T> std::ops::MulAssign<UBig<T>> for UBig<T>
where T: Digit
{
    fn mul_assign(&mut self, other: Self)
    {
        *self *= &other;
    }
}

impl<T> std::ops::MulAssign<&UBig<T>> for UBig<T>
where T: Digit
{
    fn mul_assign(&mut self, other: &UBig<T>)
    {
        *self = self.mul_big(other);
    }
}

impl<T> std::ops::Add<UBig<T>> for UBig<T>
where T: Digit
{
    type Output = UBig<T>;

    fn add(self, other: Self) -> Self::Output
    {
        &self + &other
    }
}

impl<T> std::ops::Add<&UBig<T>> for UBig<T>
where T: Digit
{
    type Output = UBig<T>;

    fn add(self, other: &UBig<T>) -> Self::Output
    {
        &self + other
    }
}

impl<T> std::ops::Add<UBig<T>> for &UBig<T>
where T: Digit
{
    type Output = UBig<T>;

    fn add(self, other: UBig<T>) -> Self::Output
    {
        self + &other
    }
}

impl<'a, 'b: 'a, T> std::ops::Add<&'b UBig<T>> for &'a UBig<T>
where T: Digit
{
    type Output = UBig<T>;

    fn add(self, other: &'b UBig<T>) -> Self::Output
    {
        let mut res = self.clone();
        res += other;
        res
    }
}

impl<T> std::ops::Sub<UBig<T>> for UBig<T>
where T: Digit
{
    type Output = UBig<T>;

    fn sub(self, other: Self) -> Self::Output
    {
        &self - &other
    }
}

impl<T> std::ops::Sub<&UBig<T>> for UBig<T>
where T: Digit
{
    type Output = UBig<T>;

    fn sub(self, other: &UBig<T>) -> Self::Output
    {
        &self - other
    }
}

impl<T> std::ops::Sub<UBig<T>> for &UBig<T>
where T: Digit
{
    type Output = UBig<T>;

    fn sub(self, other: UBig<T>) -> Self::Output
    {
        self - &other
    }
}

impl<'a, 'b: 'a, T> std::ops::Sub<&'b UBig<T>> for &'a UBig<T>
where T: Digit
{
    type Output = UBig<T>;

    fn sub(self, other: &'b UBig<T>) -> Self::Output
    {
        let mut res = self.clone();
        res -= other;
        res
    }
}

impl<T> std::ops::Mul<UBig<T>> for UBig<T>
where T: Digit
{
    type Output = UBig<T>;

    fn mul(self, other: Self) -> Self::Output
    {
        &self * &other
    }
}

impl<T> std::ops::Mul<&UBig<T>> for UBig<T>
where T: Digit
{
    type Output = UBig<T>;

    fn mul(self, other: &UBig<T>) -> Self::Output
    {
        &self * other
    }
}

impl<T> std::ops::Mul<UBig<T>> for &UBig<T>
where T: Digit
{
    type Output = UBig<T>;

    fn mul(self, other: UBig<T>) -> Self::Output
    {
        self * &other
    }
}

impl<'a, 'b: 'a, T> std::ops::Mul<&'b UBig<T>> for &'a UBig<T>
where T: Digit
{
    type Output = UBig<T>;

    fn mul(self, other: Self) -> Self::Output
    {
        self.mul_big(other)
    }
}

impl<T> std::fmt::Display for UBig<T>
where T: Digit + std::fmt::Display
{
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result
    {
        if self.is_zero()
        {
            write!(f, "0")
        }
        else if self.nr_digits() == 1
        {
            write!(f, "{}", self.digits[0])
        }
        else
        {
            let mut decimal = vec![];
            let mut tmp = self.clone();
            while !tmp.is_zero()
            {
                let mut carry = T::zero();
                for digit in tmp.digits.iter_mut().rev()
                {
                    let d = (carry.to_long() * T::RADIX) + digit.to_long();
                    carry = T::to_short(d % T::DECIMAL_RADIX);
                    *digit = T::to_short(d / T::DECIMAL_RADIX);
                }
                tmp.drop_leading_zeros();
                decimal.push(carry);
            }
            write!(f, "{}", decimal.last().unwrap())?;
            for dd in decimal.iter().rev().skip(1)
            {
                write!(f, "{:0width$}", dd, width=T::DECIMAL_WIDTH)?;
            }
            Ok(())
        }
    }
}

impl<T> std::fmt::Display for UBig<DecimalDigit<T>>
where T: std::fmt::Display, DecimalDigit<T>: Digit
{
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result
    {
        if self.is_zero()
        {
            write!(f, "0")
        }
        else
        {
            write!(f, "{}", self.digits.last().unwrap().0)?;
            for dd in self.digits.iter().rev().skip(1)
            {
                write!(f, "{:0width$}", dd.0, width=DecimalDigit::DECIMAL_WIDTH)?;
            }
            Ok(())
        }
    }
}

#[cfg(test)]
mod tests
{
    use num_traits::{One, Zero};
    use super::{UBig, Error};

    #[test]
    fn test_zero()
    {
        assert_eq!(UBig::<u32>::zero().digits(), &[] as &[u32]);
    }

    #[test]
    fn test_is_zero()
    {
        assert!(UBig::<u32>::zero().is_zero());
        assert!(!UBig::<u32>::one().is_zero());
        assert!(!UBig::<u32>::from(19872u32).is_zero());
    }

    #[test]
    fn test_one()
    {
        assert_eq!(UBig::<u32>::one().digits(), &[1]);
    }

    #[test]
    fn test_from_u32()
    {
        assert_eq!(UBig::<u32>::from(19872u32).digits(), &[19872]);
        assert_eq!(UBig::<u32>::from(0xffffffffu32).digits(), &[0xffffffff]);
    }

    #[test]
    fn test_cmp()
    {
        assert!(UBig::<u32>::zero() == UBig::<u32>::zero());
        assert!(!(UBig::<u32>::zero() < UBig::<u32>::zero()));
        assert!(!(UBig::<u32>::zero() > UBig::<u32>::zero()));

        assert!(UBig::<u32>::zero() != UBig::<u32>::one());
        assert!(!(UBig::<u32>::one() < UBig::<u32>::zero()));
        assert!(UBig::<u32>::one() > UBig::<u32>::zero());

        assert!(UBig::<u32>::one() == UBig::<u32>::one());
        assert!(!(UBig::<u32>::one() < UBig::<u32>::one()));
        assert!(!(UBig::<u32>::one() > UBig::<u32>::one()));

        assert!(UBig::<u32>::one() != UBig::<u32>::from(1234567));
        assert!(UBig::<u32>::one() < UBig::<u32>::from(1234567));
        assert!(!(UBig::<u32>::one() > UBig::<u32>::from(1234567)));

        assert!(UBig::<u32>::new(vec![0xffffffff]) != UBig::<u32>::new(vec![0, 1]));
        assert!(UBig::<u32>::new(vec![0xffffffff]) < UBig::<u32>::new(vec![0, 1]));
        assert!(!(UBig::<u32>::new(vec![0xffffffff]) > UBig::<u32>::new(vec![0, 1])));

        assert!(UBig::<u32>::new(vec![0, 0xffffffff]) != UBig::<u32>::new(vec![0xffffffff, 1]));
        assert!(!(UBig::<u32>::new(vec![0, 0xffffffff]) < UBig::<u32>::new(vec![0xffffffff, 1])));
        assert!(UBig::<u32>::new(vec![0, 0xffffffff]) > UBig::<u32>::new(vec![0xffffffff, 1]));
    }

    #[test]
    fn test_inc()
    {
        assert_eq!(UBig::<u32>::zero().inc().digits(), &[1]);
        assert_eq!(UBig::<u32>::one().inc().digits(), &[2]);
        assert_eq!(UBig::<u32>::from(19872u32).inc().digits(), &[19873]);
        assert_eq!(UBig::<u32>::from(0xffffffffu32).inc().digits(), &[0, 1]);
    }

    #[test]
    fn test_add_assign_big()
    {
        assert_eq!(UBig::<u32>::zero().add_assign_big(&UBig::zero()).digits(), &[] as &[u32]);
        assert_eq!(UBig::<u32>::zero().add_assign_big(&UBig::one()).digits(), &[1]);
        assert_eq!(UBig::<u32>::one().add_assign_big(&UBig::zero()).digits(), &[1]);
        assert_eq!(UBig::<u32>::one().add_assign_big(&UBig::one()).digits(), &[2]);
        assert_eq!(UBig::<u32>::from(19872u32).add_assign_big(&UBig::from(19872u32)).digits(), &[39744]);
        assert_eq!(UBig::<u32>::from(0xffffffffu32).add_assign_big(&UBig::from(19872u32)).digits(), &[19871, 1]);
        assert_eq!(UBig::<u32>::from(19872u32).add_assign_big(&UBig::from(0xffffffffu32)).digits(), &[19871, 1]);
        assert_eq!(UBig::<u32>::from(0xffffffffu32).add_assign_big(&UBig::from(0xffffffffu32)).digits(), &[0xfffffffe, 1]);
        assert_eq!(UBig::<u32>::from(0xffffffffu32)
                    .add_assign_big(&UBig::from(0xffffffffu32))
                    .add_assign_big(&UBig::from(0xffffffffu32))
                    .digits(), &[0xfffffffd, 2]);
    }

    #[test]
    fn test_sub_assign_big()
    {
        assert_eq!(UBig::zero().sub_assign_big(&UBig::zero()).map(|b| b.digits()), Ok(&[] as &[u32]));
        assert_eq!(UBig::<u32>::zero().sub_assign_big(&UBig::one()).map(|b| b.digits()), Err(Error::Underflow));
        assert_eq!(UBig::one().sub_assign_big(&UBig::zero()).map(|b| b.digits()), Ok(&[1u32] as &[u32]));
        assert_eq!(UBig::one().sub_assign_big(&UBig::one()).map(|b| b.digits()), Ok(&[] as &[u32]));
        assert_eq!(UBig::from(19872u32).sub_assign_big(&UBig::from(19872u32)).map(|b| b.digits()), Ok(&[] as &[u32]));
        assert_eq!(UBig::from(0xffffffffu32).sub_assign_big(&UBig::from(19872u32)).map(|b| b.digits()), Ok(&[4294947423u32] as &[u32]));
        assert_eq!(UBig::<u32>::from(19872u32).sub_assign_big(&UBig::from(0xffffffffu32)).map(|b| b.digits()), Err(Error::Underflow));
        assert_eq!(UBig::from(0xffffffffu32).sub_assign_big(&UBig::from(0xffffffffu32)).map(|b| b.digits()), Ok(&[] as &[u32]));
    }

    #[test]
    fn test_mul_big()
    {
        assert_eq!(UBig::<u32>::zero().mul_big(&UBig::zero()).digits(), &[] as &[u32]);
        assert_eq!(UBig::<u32>::zero().mul_big(&UBig::one()).digits(), &[] as &[u32]);
        assert_eq!(UBig::<u32>::one().mul_big(&UBig::zero()).digits(), &[] as &[u32]);
        assert_eq!(UBig::<u32>::one().mul_big(&UBig::one()).digits(), &[1]);
        assert_eq!(UBig::<u32>::from(19872u32).mul_big(&UBig::from(19872u32)).digits(), &[394896384]);
        assert_eq!(UBig::<u32>::from(0xffffffffu32).mul_big(&UBig::from(19872u32)).digits(), &[4294947424, 19871]);
        assert_eq!(UBig::<u32>::from(19872u32).mul_big(&UBig::from(0xffffffffu32)).digits(), &[4294947424, 19871]);
        assert_eq!(UBig::<u32>::from(0xffffffffu32).mul_big(&UBig::from(0xffffffffu32)).digits(), &[1, 0xfffffffe]);
        assert_eq!(UBig::<u32>::from(0xffffffffu32)
            .mul_big(&UBig::from(0xffffffffu32))
            .mul_big(&UBig::from(0xffffffffu32))
            .mul_big(&UBig::from(0xffffffffu32))
            .digits(), &[1, 0xfffffffc, 5, 0xfffffffc]);
        assert_eq!(UBig::<u32>::from(0xffffffffu32)
            .mul_big(&(UBig::from(0xffffffffu32))
                .mul_big(&(UBig::from(0xffffffffu32))
                    .mul_big(&UBig::from(0xffffffffu32))))
            .digits(), &[1, 0xfffffffc, 5, 0xfffffffc]);
    }

    #[test]
    fn test_multiplied2()
    {
        let n0 = UBig { digits: vec![0x13u8, 0x26u8] };
        let n1 = UBig { digits: vec![0x72u8, 0x98u8] };
        assert_eq!(n0.mul_big(&n1).digits(), &[0x76, 0x3c, 0xac, 0x16]);

        let n0 = UBig { digits: vec![0xd7u8, 0xe2, 0x4f, 0xdc] };
        let n1 = UBig { digits: vec![0xbeu8, 0x04, 0xbc, 0xcb] };
        assert_eq!(n0.mul_big(&n1).digits(), &[0x92, 0xb7, 0xb9, 0xd5, 0x9f, 0x27, 0x55, 0xaf]);

        let n0 = UBig { digits: vec![0x6du8, 0x37, 0xf2, 0x1b, 0xac, 0x04, 0xcd, 0x56] };
        let n1 = UBig { digits: vec![0x88u8, 0x15, 0xca, 0xb1, 0x58, 0x08, 0x9f, 0x83, 0x76] };
        assert_eq!(n0.mul_big(&n1).digits(), &[0xe8, 0x62, 0x3b, 0xd0, 0xa3, 0x94, 0x1e, 0xfe, 0x61, 0x1b, 0xbf, 0x55, 0xe3, 0xfc, 0x20, 0x2f, 0x28]);
    }

    #[test]
    fn test_pow()
    {
        assert_eq!(UBig::<u32>::from(10u32).pow(9).digits(), &[1_000_000_000]);
        assert_eq!(UBig::<u32>::from(10u32).pow(12).digits(), &[3567587328, 232]);
        assert_eq!(UBig::<u32>::from(36271781).pow(21).digits(), &[3008407573, 437248632, 877668550, 1371464925, 595590382, 510297384, 2424483593, 2908282777, 21647999, 1544316138, 619778902, 209469078, 3614109898, 31575924, 1137819568, 3598877581, 42032]);
        assert_eq!(UBig::<u32>::one().pow(88192891).digits(), &[1]);
        assert_eq!(UBig::<u32>::zero().pow(88192891).digits(), &[] as &[u32]);
    }
}
