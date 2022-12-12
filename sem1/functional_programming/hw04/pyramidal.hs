tetrahedral :: Int -> Int
tetrahedral n = n * (n + 1) * (n + 2) `div` 6

pyramidal :: [Int]
pyramidal = map tetrahedral [1..]
