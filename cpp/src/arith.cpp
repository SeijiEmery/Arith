#include <iostream>
#include <cstdio>

enum { TYPE_CONST = 0, TYPE_ADD, TYPE_MUL, TYPE_IF };

template <int x>
struct Const {
    enum { 
        type = TYPE_CONST, 
        result = x 
    };
    static std::ostream& repr (std::ostream& os) {
        return os << "Const<" << x << ">";
    }
    static void printf () {
        ::printf("Const<%d>", x);
    }
};

template <typename L, typename R>
struct Add {
    typedef L Left;
    typedef R Right;
    enum { 
        type = TYPE_ADD,
        result = Left::result + Right::result
    };
    static std::ostream& repr (std::ostream& os) {
        return os << "Add<",
            Left::repr(os) << ", ",
            Right::repr(os) << ">";
    }
    static void printf () {
        ::printf("Add<"); Left::printf(); ::printf(", "); Right::printf(); ::printf(">");
    }
};

template <typename L, typename R>
struct Mul {
    typedef L Left;
    typedef R Right;
    enum { 
        type = TYPE_MUL,
        result = Left::result * Right::result
    };
    static std::ostream& repr (std::ostream& os) {
        return os << "Mul<",
            Left::repr(os) << ", ",
            Right::repr(os) << ">";
    }
    static void printf () {
        ::printf("Mul<"); Left::printf(); ::printf(", "); Right::printf(); ::printf(">");
    }
};

namespace detail {

template <bool z, bool g, typename L, typename Z, typename G>
struct SelectTernary;

template <bool any, typename L, typename Z, typename G>
struct SelectTernary<true, any, L, Z, G> { typedef Z Result; };

template <typename L, typename Z, typename G>
struct SelectTernary<false, true, L, Z, G> { typedef G Result; };

template <typename L, typename Z, typename G>
struct SelectTernary<false, false, L, Z, G> { typedef L Result; };

}; // namespace detail

template <typename CL, typename CR, typename L, typename Z, typename G>
struct If {
    typedef CL ConditionLeft;
    typedef CR ConditionRight;
    typedef L  IfLess;
    typedef Z  IfZero;
    typedef G  IfGreater;

    typedef typename detail::SelectTernary<
        (CL::result == CR::result), 
        (CL::result > CR::result), 
        L, Z, G
    >::Result Result;
    
    enum {
        type   = TYPE_IF,
        result = Result::result
    };

    static std::ostream& repr (std::ostream& os) {
        return os << "If<",
            ConditionLeft::repr(os) << " <-> ",
            ConditionRight::repr(os) << " => ",
            // os << "",
            IfLess::repr(os) << ", ",
            IfZero::repr(os) << ", ",
            IfGreater::repr(os) << ">";
    }
    static void printf () {
        ::printf("If<");
        ConditionLeft::printf(); ::printf(" == ");
        ConditionRight::printf(); ::printf(" then ");
    }
};

#define eval(Expr) (Expr::result)
#define repr(Expr) (Expr::repr(std::cout))
#define print(Expr) (Expr::printf())

#if defined(NO_OUTPUT)
    #define printEval(Expr)
#elif defined(USE_IOSTREAM)
    #define printEval(Expr) (repr(Expr) << " => " << eval(Expr) << '\n')
#else // defined(USE_PRINTF)
    #define printEval(Expr) (print(Expr), printf(" => %d\n", eval(Expr)))
#endif

#define assertEvaluatesTo(Expr, result) \
    static_assert(eval(Expr) == result, #Expr " !=> " #result)

int main () {
    using A = Const<12>;
    assertEvaluatesTo(A, 12);
    printEval(A);

    using If_12_2 = If<Const<12>, Const<2>, Const<-1>, Const<0>, Const<1>>;
    using If_2_12 = If<Const<2>, Const<12>, Const<-1>, Const<0>, Const<1>>;
    using If_12_12 = If<Const<12>, Const<12>, Const<-1>, Const<0>, Const<1>>;
    printEval(If_12_2);
    assertEvaluatesTo(If_12_2, +1);
    printEval(If_2_12);
    assertEvaluatesTo(If_2_12, -1);
    printEval(If_12_12);
    #ifdef INDUCE_ERROR
        assertEvaluatesTo(If_12_12, 1);
    #else
        assertEvaluatesTo(If_12_12, 0);
    #endif

    using B  = Add<Const<2>, Const<3>>;
    using B_ = Add<Const<3>, Const<2>>;
    #ifndef INDUCE_ERROR
        assertEvaluatesTo(B,  5);
        assertEvaluatesTo(B_, 5);
    #else
        assertEvaluatesTo(B,  22);
    #endif
    printEval(B);
    printEval(B_);

    using C  = Mul<B, Const<11>>;
    using C_ = Mul<Const<11>, B>;
    assertEvaluatesTo(C, 55);
    assertEvaluatesTo(C_, 55);
    printEval(C);
    printEval(C_);

    using D  = Add<A, C>;
    using D_ = Add<C, A>;
    #ifndef INDUCE_ERROR
        assertEvaluatesTo(D, 67);
        assertEvaluatesTo(D_, 67);
    #else
        assertEvaluatesTo(D_, 62);
    #endif
    printEval(D);
    printEval(D_);
}