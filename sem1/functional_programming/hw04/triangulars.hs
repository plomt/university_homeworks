triangular :: Int -> Int
triangular n = n * (n - 1) `div` 2

triangulars :: [Int]
triangulars = map triangular [1..]

