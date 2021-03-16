use num_traits::{Zero, One};
use crate::digit::Digit;

/// Type wrapper for describing digits in a decimal based number. The type parameter `T`
/// is the underlying representation of the digit.
#[derive(Clone, Copy, Debug, PartialEq, PartialOrd)]
pub struct DecimalDigit<T>(pub T);

impl<T> std::ops::AddAssign for DecimalDigit<T>
where T: std::ops::AddAssign
{
    fn add_assign(&mut self, other: Self)
    {
        self.0 += other.0;
    }
}

impl<T> std::ops::DivAssign for DecimalDigit<T>
where T: std::ops::DivAssign
{
    fn div_assign(&mut self, other: Self)
    {
        self.0 /= other.0;
    }
}

impl<T> std::ops::MulAssign for DecimalDigit<T>
where T: std::ops::MulAssign
{
    fn mul_assign(&mut self, other: Self)
    {
        self.0 *= other.0;
    }
}

impl<T> std::ops::RemAssign for DecimalDigit<T>
where T: std::ops::RemAssign
{
    fn rem_assign(&mut self, other: Self)
    {
        self.0 %= other.0;
    }
}

impl<T> std::ops::SubAssign for DecimalDigit<T>
where T: std::ops::SubAssign
{
    fn sub_assign(&mut self, other: Self)
    {
        self.0 -= other.0;
    }
}

impl<T> std::ops::Add for DecimalDigit<T>
where T: std::ops::Add<Output=T>
{
    type Output = Self;

    fn add(self, other: Self) -> Self::Output
    {
        DecimalDigit(self.0 + other.0)
    }
}

impl<T> std::ops::Mul for DecimalDigit<T>
where T: std::ops::Mul<Output=T>
{
    type Output = Self;

    fn mul(self, other: Self) -> Self::Output
    {
        DecimalDigit(self.0 * other.0)
    }
}

impl<T> Digit for DecimalDigit<T>
where T: Digit
{
    type LongDigitType = T::LongDigitType;

    const RADIX: Self::LongDigitType = T::DECIMAL_RADIX;
    const MAX: Self = DecimalDigit(T::DECIMAL_MAX);
    const DECIMAL_RADIX: Self::LongDigitType = T::DECIMAL_RADIX;
    const DECIMAL_MAX: Self = DecimalDigit(T::DECIMAL_MAX);
    const DECIMAL_WIDTH: usize = T::DECIMAL_WIDTH;

    fn as_long(self) -> Self::LongDigitType
    {
        self.0.as_long()
    }

    fn as_short(long: Self::LongDigitType) -> Self
    {
        DecimalDigit(T::as_short(long))
    }
}

impl<T> Zero for DecimalDigit<T>
where T: Zero + std::ops::Add<Output=T>
{
    fn zero() -> Self
    {
        DecimalDigit(T::zero())
    }

    fn is_zero(&self) -> bool
    {
        self.0.is_zero()
    }
}

impl<T> One for DecimalDigit<T>
where T: One + std::ops::Mul<Output=T>
{
    fn one() -> Self
    {
        DecimalDigit(T::one())
    }
}
