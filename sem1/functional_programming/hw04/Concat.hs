concat' :: [[a]] -> [a]
concat' l = foldr (++) [] l
