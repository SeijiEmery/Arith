#include <iostream>
#include <cstdio>

// Tags for traversing the arith AST
enum { TYPE_CONST = 0, TYPE_ADD, TYPE_MUL, TYPE_IF };

// AST elements are c++ types.
// I am -sort of- cheating by directly / eagerly evaluating the AST's result,
// but this is the only way to do this with templates. FWIW, it's still fully
// possible to walk the AST after the fact since all AST nodes have references
// stored to them (see Left, Right, etc).

template <int x>                // argument (a 32-bit integer)
struct Const {
    enum { 
        type = TYPE_CONST,      // type tag. useful only for traversal; unused here
        result = x              // store value (technically, we're eagerly evaluating our results)
    };

    // stringification, implemented for both std::ostream and C's printf()
    static std::ostream& repr (std::ostream& os) {
        return os << "Const<" << x << ">";
    }
    static void printf () {
        ::printf("Const<%d>", x);
    }
};

template <typename L, typename R>
struct Add {
    typedef L Left;     // store results. Accessible as (this-type)::Left, (this-type)::Right.
    typedef R Right;    // these are assumed to be other Arith AST nodes, and can be of any type.
    enum { 
        type = TYPE_ADD,                        // type tag
        result = Left::result + Right::result   // again, results are eagerly evaluated
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

// New feature: ternary If.
// usage:
//   If< Cond1, Cond2, Lesser, Zero, Greater >
//
// Evaluates Cond1 and Cond2, then:
//  if Cond1 < Cond2, evaluates and returns Lesser
//  if Cond1 = Cond2, evaluates and returns Zero
//  if Cond1 > Cond2, evaluates and returns Greater
//
// where Cond1, Cond2, Lesser, Zero, and Greater are all Arith expressions (AST nodes).

// Helper: implement ternary selection / branching
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

// Macros for evaluation and stringification / printing.
// Again, I'm sort of "cheating" by eagerly evaluating AST nodes as they are constructed,
// though it is concievable that a language implementation could do this.

// evaluates an expression, returning its evaluated result
#define eval(Expr) (Expr::result)

// prints an expression to std::cout using Expr::repr
#define repr(Expr) (Expr::repr(std::cout))

// prints an expression to standard output using Expr::printf
#define print(Expr) (Expr::printf())

// printEval prints an expression followed by what it evaluates to.
//
// The implementation can be changed via compiler flags:
//  -DNO_OUTPUT,
//  -DUSE_IOSTREAM, or
//  -DUSE_PRINTF
//
// By default it will use printf.

#if defined(NO_OUTPUT)
    #define printEval(Expr)
#elif defined(USE_IOSTREAM)
    #define printEval(Expr) (repr(Expr) << " => " << eval(Expr) << '\n')
#else // defined(USE_PRINTF)
    #define printEval(Expr) (print(Expr), printf(" => %d\n", eval(Expr)))
#endif

// asserts that an expression evaluates to result, checked at compile time using
// static_assert. Will print a somewhat helpful error message if it fails.
//
// (If you are doing assertEvaluatesTo(Mul<Add<Const<2>, Const<3>>, Const<4>>, 21),
//  you will get a somewhat helpful message that this does not evaluate to 21.
//  If you aliased the type as using Foo = ...; assertEvaluatesTo(Foo, 21), this
//  message is... somewhat less helpful)
//
#define assertEvaluatesTo(Expr, result) \
    static_assert(eval(Expr) == result, #Expr " !=> " #result)

int main () {
    // Testcases. The If testcase is somewhat long yet basic, but obviously 
    // could be combined with any of the other expressions here.

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