fib n = go 0 1 n
  where go a b acc | acc == 0  = a
                   | otherwise = go b (a + b) (acc - 1)
