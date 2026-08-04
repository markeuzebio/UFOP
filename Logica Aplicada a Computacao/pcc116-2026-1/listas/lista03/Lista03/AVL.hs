module AVL
  ( AVL
  , empty
  , singleton
  , insert
  , delete
  , member
  , toList
  , fromList
  , height
  , size
  , minElem
  , maxElem
  ) where

-- Tipo de dados

data AVL a
  = Leaf
  | Node
      { avlHeight :: !Int
      , avlLeft   :: !(AVL a)
      , avlVal    :: !a
      , avlRight  :: !(AVL a)
      }
  deriving (Eq)

-- Instâncias

instance Show a => Show (AVL a) where
  show t = "fromList " ++ show (toList t)

instance Foldable AVL where
  foldr _ z Leaf           = z
  foldr f z (Node _ l v r) = foldr f (f v (foldr f z r)) l

-- Primitivas internas

height :: AVL a -> Int
height Leaf           = 0
height (Node h _ _ _) = h

node :: AVL a -> a -> AVL a -> AVL a
node l v r = Node (1 + max (height l) (height r)) l v r

balance :: AVL a -> Int
balance Leaf           = 0
balance (Node _ l _ r) = height r - height l

-- Rotações

rotateRight :: AVL a -> AVL a
rotateRight (Node _ (Node _ ll lv lr) v r) =
  node ll lv (node lr v r)
rotateRight t = t

rotateLeft :: AVL a -> AVL a
rotateLeft (Node _ l v (Node _ rl rv rr)) =
  node (node l v rl) rv rr
rotateLeft t = t

rebalance :: AVL a -> AVL a
rebalance Leaf = Leaf
rebalance t@(Node _ l v r)
  | balance t == -2 =
      if balance l <= 0
        then rotateRight t
        else rotateRight (node (rotateLeft l) v r)
  | balance t ==  2 =
      if balance r >= 0
        then rotateLeft t
        else rotateLeft (node l v (rotateRight r))
  | otherwise = t

-- API pública

empty :: AVL a
empty = Leaf

singleton :: a -> AVL a
singleton v = node Leaf v Leaf

size :: AVL a -> Int
size = foldr (\_ acc -> acc + 1) 0

member :: Ord a => a -> AVL a -> Bool
member _ Leaf = False
member x (Node _ l v r)
  | x < v     = member x l
  | x > v     = member x r
  | otherwise  = True

insert :: Ord a => a -> AVL a -> AVL a
insert x Leaf = singleton x
insert x t@(Node _ l v r)
  | x < v     = rebalance (node (insert x l) v r)
  | x > v     = rebalance (node l v (insert x r))
  | otherwise  = t

deleteMin :: AVL a -> (a, AVL a)
deleteMin (Node _ Leaf v r) = (v, r)
deleteMin (Node _ l    v r) =
  let (m, l') = deleteMin l
  in  (m, rebalance (node l' v r))
deleteMin Leaf = error "deleteMin: árvore vazia"

delete :: Ord a => a -> AVL a -> AVL a
delete _ Leaf = Leaf
delete x (Node _ l v r)
  | x < v     = rebalance (node (delete x l) v r)
  | x > v     = rebalance (node l v (delete x r))
  | otherwise  =
      case r of
        Leaf -> l
        _    -> let (m, r') = deleteMin r
                in  rebalance (node l m r')

toList :: AVL a -> [a]
toList = foldr (:) []

fromList :: Ord a => [a] -> AVL a
fromList = foldr insert empty

minElem :: AVL a -> a
minElem Leaf              = error "minElem: árvore vazia"
minElem (Node _ Leaf v _) = v
minElem (Node _ l    _ _) = minElem l

maxElem :: AVL a -> a
maxElem Leaf              = error "maxElem: árvore vazia"
maxElem (Node _ _ v Leaf) = v
maxElem (Node _ _ _ r   ) = maxElem r

-- Demonstração

demo :: IO ()
demo = do
  putStrLn "=== Demonstração de Árvore AVL ==="

  let t0 = fromList [5, 3, 7, 1, 4, 6, 8, 2] :: AVL Int
  putStrLn $ "\nÁrvore criada com fromList [5,3,7,1,4,6,8,2]:"
  putStrLn $ "  toList  : " ++ show (toList t0)
  putStrLn $ "  height  : " ++ show (height t0)
  putStrLn $ "  size    : " ++ show (size t0)
  putStrLn $ "  minElem : " ++ show (minElem t0)
  putStrLn $ "  maxElem : " ++ show (maxElem t0)

  let t1 = insert 0 t0
  putStrLn $ "\nApós insert 0:"
  putStrLn $ "  toList  : " ++ show (toList t1)
  putStrLn $ "  height  : " ++ show (height t1)

  let t2 = delete 5 t1
  putStrLn $ "\nApós delete 5 (raiz original):"
  putStrLn $ "  toList  : " ++ show (toList t2)
  putStrLn $ "  height  : " ++ show (height t2)

  putStrLn $ "\nPertinência:"
  putStrLn $ "  member 4 t0 = " ++ show (member 4 t0)
  putStrLn $ "  member 9 t0 = " ++ show (member 9 t0)

  putStrLn "\n=== Teste de rotações (inserção em sequência crescente) ==="
  let seqTree = fromList [1..15] :: AVL Int
  putStrLn $ "  fromList [1..15]  →  height = " ++ show (height seqTree)
  putStrLn   "  (altura ótima para 15 nós é 4)"
