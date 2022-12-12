import Data.Function (fix)

memoize :: (Int -> a) -> (Int -> a)
memoize f = (map f [0..] !!)

fib :: (Int -> Integer) -> Int -> Integer
fib f 0 = 1
fib f 1 = 1
fib f n = f (n - 1) + f (n - 2)

fibMemo :: Int -> Integer
fibMemo = fix (memoize . fib)



