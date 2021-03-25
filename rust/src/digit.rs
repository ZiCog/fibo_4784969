use num_traits::{NumAssignOps, NumOps, Zero, One};

/// Trait for a "wide" digit, taking up two regular digits
pub trait LongDigit<T>: Copy + NumAssignOps + One + PartialOrd + Zero
    + std::ops::Add<Self, Output=Self>
    + std::ops::BitAnd<Self, Output=Self>
    + std::ops::BitOr<Self, Output=Self>
    + std::ops::Div<Self, Output=Self>
    + std::ops::Rem<Self, Output=Self>
    + std::ops::Mul<Self, Output=Self>
    + std::ops::Sub<Self, Output=Self>
    + std::ops::Shl<usize, Output=Self>
    + std::ops::Shr<usize, Output=Self>
{
    /// Convert a wide digit to its regular digit type. Not the value to be converted should be
    /// less than the radix of the number type.
    fn to_short(self) -> T;
}

impl LongDigit<u8> for u16
{
    fn to_short(self) -> u8
    {
        self as u8
    }
}

impl LongDigit<u16> for u32
{
    fn to_short(self) -> u16
    {
        self as u16
    }
}

impl LongDigit<u32> for u64
{
    fn to_short(self) -> u32
    {
        self as u32
    }
}

impl LongDigit<u64> for u128
{
    fn to_short(self) -> u64
    {
        self as u64
    }
}

/// Trait for types that can represent a digit in a big number
pub trait Digit: Copy + NumOps + NumAssignOps + One + PartialOrd + Zero
    + std::ops::Shl<u32, Output=Self>
    + std::fmt::Debug
{
    /// "Wide" digit type that can represent `r<sup>2</sup>` different values, where `r` is the
    /// radix of the number type. Typically twice as wide as the digit type.
    type LongDigitType: LongDigit<Self>;

    /// The radix of the number type
    const RADIX: Self::LongDigitType;
    /// The maximum value of a digit. Should be equal to `RADIX-1`.
    const MAX: Self;
    /// The number of hexadecimal digits in `MAX`
    const HEXADECIMAL_WIDTH: usize;
    /// Maximum power of 10 below which all values can be represented by a single digit
    const DECIMAL_RADIX: Self::LongDigitType;
    /// The maximum digit value when representing a decimal number. Should be equal to `DECIMAL_RADIX-1`.
    const DECIMAL_MAX: Self;
    /// The number of decimal digits in `DECIMAL_MAX`
    const DECIMAL_WIDTH: usize;

    /// Convert a digit to its wide representation
    fn to_long(self) -> Self::LongDigitType;

    /// Maximum number of places this digit can be shifted left before it overflows.
    fn max_shift(self) -> u32;
}

impl Digit for u8
{
    type LongDigitType = u16;

    const RADIX: Self::LongDigitType = 0x100;
    const MAX: Self = 0xff;
    const HEXADECIMAL_WIDTH: usize = 2;
    const DECIMAL_RADIX: Self::LongDigitType = 100;
    const DECIMAL_MAX: Self = 99;
    const DECIMAL_WIDTH: usize = 2;

    fn to_long(self) -> Self::LongDigitType
    {
        self as u16
    }

    fn max_shift(self) -> u32
    {
        self.leading_zeros()
    }
}

impl Digit for u16
{
    type LongDigitType = u32;

    const RADIX: Self::LongDigitType = 0x10000;
    const MAX: Self = 0xffff;
    const HEXADECIMAL_WIDTH: usize = 4;
    const DECIMAL_RADIX: Self::LongDigitType = 10_000;
    const DECIMAL_MAX: Self = 9_999;
    const DECIMAL_WIDTH: usize = 4;

    fn to_long(self) -> Self::LongDigitType
    {
        self as u32
    }

    fn max_shift(self) -> u32
    {
        self.leading_zeros()
    }
}

impl Digit for u32
{
    type LongDigitType = u64;

    const RADIX: Self::LongDigitType = 0x100000000;
    const MAX: Self = 0xffffffff;
    const HEXADECIMAL_WIDTH: usize = 8;
    const DECIMAL_RADIX: Self::LongDigitType = 1_000_000_000;
    const DECIMAL_MAX: Self = 999_999_999;
    const DECIMAL_WIDTH: usize = 9;

    fn to_long(self) -> Self::LongDigitType
    {
        self as u64
    }

    fn max_shift(self) -> u32
    {
        self.leading_zeros()
    }
}

impl Digit for u64
{
    type LongDigitType = u128;

    const RADIX: Self::LongDigitType = 0x10000000000000000;
    const MAX: Self = 0xffffffffffffffff;
    const HEXADECIMAL_WIDTH: usize = 16;
    const DECIMAL_RADIX: Self::LongDigitType = 10_000_000_000_000_000_000;
    const DECIMAL_MAX: Self = 9_999_999_999_999_999_999;
    const DECIMAL_WIDTH: usize = 19;

    fn to_long(self) -> Self::LongDigitType
    {
        self as u128
    }

    fn max_shift(self) -> u32
    {
        self.leading_zeros()
    }
}
