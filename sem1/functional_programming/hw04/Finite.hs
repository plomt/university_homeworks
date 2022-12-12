import Data.SInt (SInt)

class Finite a where 
    type Cardinality a :: Nat
    
    cardinality' :: SC a (Cardinality a)
    fromFin :: Fin (Cardinality a) -> a

    newtype SC a n = SC {getSC :: SInt n }

    cardinality :: forall a. Finite a => SInt (Cardinality a)
    cardinality = getSC (cardinality' @a)

    enumerate :: forall a. Finite a => [a]
    enumerate = fromFin <$> enumFin (cardinality @a)

