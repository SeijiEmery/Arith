#!/usr/bin/env python3

class Const:
    def __init__ (self, value):
        self.value = int(value)

    def eval (self):
        return self.value

    def __repr__ (self):
        return "Const(%d)"%self.value


class Add:
    def __init__ (self, left, right):
        self.left, self.right = left, right

    def eval (self):
        return self.left.eval() + self.right.eval()

    def __repr__ (self):
        return "Add(%s, %s)"%(self.left, self.right)


class Mul:
    def __init__ (self, left, right):
        self.left, self.right = left, right

    def eval (self):
        return self.left.eval() * self.right.eval()

    def __repr__ (self):
        return "Mul(%s, %s)"%(self.left, self.right)


class If:
    def __init__ (self, cond1, cond2, iflesser, ifequal, ifgreater):
        self.cond1, self.cond2, self.iflesser, self.ifequal, self.ifgreater = \
            cond1, cond2, iflesser, ifequal, ifgreater

    def eval (self):
        x1, x2 = self.cond1.eval(), self.cond2.eval()
        def select (zero, greater):
            if zero:
                return self.ifequal
            elif greater:
                return self.ifgreater
            return self.iflesser
        return select((x1 == x2), (x1 > x2)).eval()

    def __repr__ (self):
        return "If(%s, %s, %s, %s, %s)"%(
            self.cond1, self.cond2, self.iflesser, self.ifequal, self.ifgreater
        )


def printEval (expr):
    print("%s => %s"%(expr, expr.eval()))


if __name__ == '__main__':
    A = Const(12)
    printEval(A)

    If_12_2 = If(Const(12), Const(2), Const(-1), Const(0), Const(1))
    If_2_12 = If(Const(2), Const(12), Const(-1), Const(0), Const(1))
    If_12_12 = If(Const(12), Const(12), Const(-1), Const(0), Const(1))
    
    printEval(If_12_2)
    printEval(If_2_12)
    printEval(If_12_12)

    B  = Add(Const(2), Const(3))
    B_ = Add(Const(3), Const(2))

    printEval(B)
    printEval(B_)

    C  = Mul(B, Const(11))
    C_ = Mul(Const(11), B)

    printEval(C)
    printEval(C_)

    D  = Add(A, C)
    D_ = Add(C, A)

    printEval(D)
    printEval(D_)
