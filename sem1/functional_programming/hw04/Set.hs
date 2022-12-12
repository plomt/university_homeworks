data Set a = Empty | Set a (Set a) deriving (Show, Read)

instance (Eq a) => Eq (Set a)  where
    thisSet == otherSet = thisSet `subset` otherSet && otherSet `subset` thisSet

fromList :: [a] -> Set a
fromList []     = Empty
fromList (x:xs) =  Set x (fromList xs) 

head :: Set a -> a
head Empty     = error "head: empty set!"
head (Set x _) = x

tail :: Set a -> Set a
tail Empty      = Empty
tail (Set _ xs) = xs 

member :: (Eq a) => a -> Set a -> Bool
member _ Empty		        = False
member a (Set x xs) | a == x    = True
		    | otherwise = member a xs

subset :: (Eq a) => Set a -> Set a -> Bool
subset Empty _                                   = True
subset _ Empty                                   = False
subset Empty Empty                               = True
subset (Set x xs) otherSet | x `member` otherSet = subset xs otherSet
                           | otherwise           = False
