
data Expr
    = Const Integer
    | Add Expr Expr
    | Mul Expr Expr
    | If Expr Expr Expr Expr Expr
    deriving (Show)

eval :: Expr -> Integer
eval (Const x) = x
eval (Add left right) = (eval left) + (eval right)
eval (Mul left right) = (eval left) * (eval right)
eval (If cond1 cond2 lesser zero greater) 
    = eval $ select (x1 == x2) (x1 > x2) lesser zero greater
    where 
        x1 = eval cond1
        x2 = eval cond2
        select True _       lesser zero greater = zero
        select False True   lesser zero greater = greater
        select False False  lesser zero greater = lesser

printEval :: Expr -> IO ()
printEval expr = putStrLn $ (show expr) ++ " => " ++ (show . eval $ expr)

main :: IO ()
main = do
    printEval (Const 12)
    printEval (If (Const 12) (Const 2) (Const (-1)) (Const 0) (Const 1))
    printEval (If (Const 2) (Const 12) (Const (-1)) (Const 0) (Const 1))
    printEval (If (Const 12) (Const 12) (Const (-1)) (Const 0) (Const 1))
    printEval (Add (Const 2) (Const 3))
    printEval (Add (Const 3) (Const 2))
    printEval (Mul (Add (Const 3) (Const 2)) (Const 11))
    printEval (Mul (Const 11) (Add (Const 3) (Const 2)))
    printEval (Add (Mul (Const 11) (Add (Const 3) (Const 2))) (Const 12))
    printEval (Add (Const 12) (Mul (Const 11) (Add (Const 3) (Const 2))))
