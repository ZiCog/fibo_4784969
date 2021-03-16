use num_traits::{NumAssignOps, Zero, One};

/// Trait for types that can represent a digit in a big number
pub trait Digit: Copy + NumAssignOps + One + PartialOrd + Zero
    + std::fmt::Debug
{
    /// "Long" digit type that can represent `r<sup>2</sup>` different values, where `r` is the
    /// radix of the number type. Typically twice as wide as the digit type.
    type LongDigitType: Copy + NumAssignOps + One + Zero
        + std::ops::Add<Self::LongDigitType, Output=Self::LongDigitType>
        + std::ops::Div<Self::LongDigitType, Output=Self::LongDigitType>
        + std::ops::Rem<Self::LongDigitType, Output=Self::LongDigitType>
        + std::ops::Mul<Self::LongDigitType, Output=Self::LongDigitType>
        + std::ops::Sub<Self::LongDigitType, Output=Self::LongDigitType>
        + std::ops::Shl<usize, Output=Self::LongDigitType>;

    /// The radix of the number type
    const RADIX: Self::LongDigitType;
    /// The maximum value of a digit. Should be equal to `RADIX-1`.
    const MAX: Self;
    /// Maximum power of 10 below which all values can be represented by a single digit
    const DECIMAL_RADIX: Self::LongDigitType;
    /// The maximum digit value when representing a decimal number. Should be equal to `DECIMAL_RADIX-1`.
    const DECIMAL_MAX: Self;
    /// The number of decimal digits in `DECIMAL_MAX`
    const DECIMAL_WIDTH: usize;

    /// Convert a digit to its wide representation
    fn to_long(self) -> Self::LongDigitType;
    /// Convert a wide digit to its normal representation. The value should be less than `RADIX`.
    fn to_short(long: Self::LongDigitType) -> Self;
}

impl Digit for u8
{
    type LongDigitType = u16;

    const RADIX: Self::LongDigitType = 0x100;
    const MAX: Self = 0xff;
    const DECIMAL_RADIX: Self::LongDigitType = 100;
    const DECIMAL_MAX: Self = 99;
    const DECIMAL_WIDTH: usize = 2;

    fn to_long(self) -> Self::LongDigitType
    {
        self as u16
    }

    fn to_short(long: Self::LongDigitType) -> Self
    {
        long as u8
    }
}

impl Digit for u16
{
    type LongDigitType = u32;

    const RADIX: Self::LongDigitType = 0x10000;
    const MAX: Self = 0xffff;
    const DECIMAL_RADIX: Self::LongDigitType = 10_000;
    const DECIMAL_MAX: Self = 9_999;
    const DECIMAL_WIDTH: usize = 4;

    fn to_long(self) -> Self::LongDigitType
    {
        self as u32
    }

    fn to_short(long: Self::LongDigitType) -> Self
    {
        long as u16
    }
}

impl Digit for u32
{
    type LongDigitType = u64;

    const RADIX: Self::LongDigitType = 0x100000000;
    const MAX: Self = 0xffffffff;
    const DECIMAL_RADIX: Self::LongDigitType = 1_000_000_000;
    const DECIMAL_MAX: Self = 999_999_999;
    const DECIMAL_WIDTH: usize = 9;

    fn to_long(self) -> Self::LongDigitType
    {
        self as u64
    }

    fn to_short(long: Self::LongDigitType) -> Self
    {
        long as u32
    }
}

impl Digit for u64
{
    type LongDigitType = u128;

    const RADIX: Self::LongDigitType = 0x10000000000000000;
    const MAX: Self = 0xffffffffffffffff;
    const DECIMAL_RADIX: Self::LongDigitType = 10_000_000_000_000_000_000;
    const DECIMAL_MAX: Self = 9_999_999_999_999_999_999;
    const DECIMAL_WIDTH: usize = 19;

    fn to_long(self) -> Self::LongDigitType
    {
        self as u128
    }

    fn to_short(long: Self::LongDigitType) -> Self
    {
        long as u64
    }
}
