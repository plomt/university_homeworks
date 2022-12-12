countChange :: Int -> [Int] -> Int
countChange amount coins
    | amount == 0 = 1
    | amount < 0  = 0
    | otherwise   = case coins of
        []   -> 0
        c:cs -> countChange (amount - c) coins + countChange amount cs
