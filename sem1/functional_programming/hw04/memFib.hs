memFib :: Int -> Integer
memFib n = (map fib [0..] !!) n
  where fib 0 = 1
        fib 1 = 1
        fib n = memFib (n - 2) + memFib (n - 1)
