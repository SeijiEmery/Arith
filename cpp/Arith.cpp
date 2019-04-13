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

// primary template to compute sqrt(N)
template <int N, int LO=1, int HI=N>
class Sqrt {
  public:
    // compute the midpoint, rounded up
    enum { mid = (LO+HI+1)/2 };

    // search a not too large value in a halved interval
    enum { result = (N<mid*mid) ? Sqrt<N,LO,mid-1>::result
                                : Sqrt<N,mid,HI>::result };
};

// partial specialization for the case when LO equals HI
template<int N, int M>
class Sqrt<N,M,M> {
  public:
    enum { result = M };
};

template <int k>
struct BPP {
    typedef 
};








template <int k, int base = 10000>
struct BBP {
    enum {
        result = /*(base / (1 << (k << 2))) * */ (
            (4 * base) / (8 * k + 1) -
            (2 * base) / (8 * k + 4) -
            (1 * base) / (8 * k + 5) -
            (1 * base) / (8 * k + 6)
        ) //+ BBP<k - 1, base>::result
        // result = (base / (1 << (k << 2))) * (
        //     (4 * base) / (8 * k + 1) -
        //     (2 * base) / (8 * k + 4) -
        //     (1 * base) / (8 * k + 5) -
        //     (1 * base) / (8 * k + 6)
        // ) + BBP<k - 1, base>::result
    };
};
template <int base>
struct BBP<-1, base> {
    enum { result = 0 };
};

template<int count, int x = 1, int k = 10>
struct PiPartial {
    enum {
        result = k * 2 - Sqrt<
            k * k * 4 - PiPartial<count - 1, x, k * k>::result
        >::result
    };
};
template <int x, int k>
struct PiPartial<0, x, k> {
    enum {  result = x };
};
template <int count>
struct Pi {
    enum {
        result = BBP<count>::result
        // result = 3 * (1 << count) * Sqrt<PiPartial<count>::result>::result
    };
};

#define eval(Expr) (Expr::result)
#define repr(Expr) (Expr::repr(std::cout))
#define print(Expr) (Expr::printf())
#define printEval(Expr) (repr(Expr) << " => " << eval(Expr) << '\n')
// #define printEval(Expr) (std::cout << " => " << eval(Expr) << '\n')
// #define printEval(Expr) (print(Expr), printf(" => %d\n", eval(Expr)))
// #define printEval(Expr)
// #define printEval(Expr) (printf(" => %d\n", eval(Expr)))
#define assertEvaluatesTo(Expr, result) \
    static_assert(eval(Expr) == result, #Expr " !=> " #result)

int main () {
    #define PRINT_PI_APPROX(n) \
        printf("pi ~= %d at %d iterations\n", \
            Pi<n>::result, n);


        // printf("at %d, p = %d, pi = %d\n", n, \
        //     PiPartial<n>::result, 0), \
        //     Pi<n>::result);

    PRINT_PI_APPROX(0)
    PRINT_PI_APPROX(1)
    PRINT_PI_APPROX(2)
    PRINT_PI_APPROX(3)
    PRINT_PI_APPROX(4)
    PRINT_PI_APPROX(5)
    PRINT_PI_APPROX(6)
    PRINT_PI_APPROX(100)

    assertEvaluatesTo(Sqrt<255>, 15);

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
    assertEvaluatesTo(If_12_12, 0);

    using B  = Add<Const<2>, Const<3>>;
    using B_ = Add<Const<3>, Const<2>>;
    assertEvaluatesTo(B, 5);
    assertEvaluatesTo(B_, 5);
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
    assertEvaluatesTo(D, 67);
    assertEvaluatesTo(D_, 67);
    // assertEvaluatesTo(D_, 62);
    printEval(D);
    printEval(D_);
}