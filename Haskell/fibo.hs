import System.Environment
import Data.Bits
import Data.List

-- Basic definition
fibo :: Int -> Integer
fibo n
    | n < 0     = -fibo (-n)
    | n < 2     = toInteger n
    | otherwise = fibo (n - 1) + fibo (n - 2)


-- A faster version
fiboFast :: Int -> Integer
fiboFast n
    | n < 0          = -fiboFast (-n)
    | n < 2          = toInteger n
    | even n         = a * (2 * b + a)
    | n `mod` 4 == 1 = (2 * a + b) * (2 * a - b) + 2
    | otherwise      = (2 * a + b) * (2 * a - b) - 2
    where
        k = n `div` 2
        a = fiboFast k
        b = fiboFast (k - 1)


-- Even faster.
-- Uses shiftR from Data.Bits rather than dividing by 2.
-- Uses foldl' from Data.List which is greedy rather than the lazy foldl
-- toBinaryList creates a list of True/False representing n in binary
--   which fiboFaster' uses to decide which pair to evaluate.
fiboFaster :: Int -> Integer
fiboFaster n
    | n < 0     = -fiboFaster (-n)
    | n < 2     = toInteger n
    | otherwise = snd . foldl' fiboFaster' (1, 0) $ toBinaryList n
    where
        toBinaryList 0 = []
        toBinaryList n = toBinaryList (n `shiftR` 1) ++ [odd n]

        fiboFaster' (a, b) bitset
            | bitset    = (a * (2 * b + a), a2b2)
            | otherwise = (a2b2,  b * (2 * a - b))
            where a2b2 = a ^ 2 + b ^ 2


main :: IO ()
main = do {
    args <- getArgs;
    print (fiboFaster (if length args == 0 then
                           4784969
                       else
                           read (head args)::Int));
}
