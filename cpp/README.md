# Compile-time Arith

I've implemented the toy language Arith using c++ templates. The neat thing about this is that Arith runs (effectively) at compile time: evaluation can be done at compile time using static_assert (which produces compiler errors with somewhat useful compilation messages). 

Technically that is sufficient to say that it "runs at compile time", though I've also added helpful pretty printing (both using iostreams and printf), and that of course must run at runtime and generates some code.

Since it was interesting to compare the outputs I've added assembly outputs to the makefile. (it can switch between no output / printf output / iostream output via a #define / -D flag). Note that with no output it generates effectively nothing (just a main() function that returns), and that the printf() version is substantially more efficient (or at least readable), which is why I added it. The definitions are of course still there in the c++ source, but the c++ compiler is smart enough to not generate code for templated code that never gets used, so switching between these modes was as simple as changing one macro definition.

The full implementation is in src/arith.cpp

## Build instructions:

    git clone https://github.com/SeijiEmery/arith.git
    cd arith/cpp
    make

This will build and run all examples (no output / printf / iostream only). Assembly output (in the form of .s files) is in build/.

To see an example of what happens (at compile time) when there is an error, run

    make with_error

Or alternatively just edit the examples in main() yourself.

Note: by default `make` will also build and run the haskell version, which assumes that you have GHC installed.

## Haskell + Python

There is a direct translation of the c++ source to haskell in src/arith.hs, and this should produce the same exact output (with some form of printing enabled) as the c++ version (albeit with '()' instead of '<>').

There is also a python version in src/arith.py, which again is a direct translation.

These aren't meant as standalone implementations (as standalone implementations they're both somewhat suboptimal), but to demonstrate (and compare with) how the c++ code works.

## Editing:

If you have [when-changed](https://github.com/joh/when-changed) installed, you can run `make interactive` (and hit enter), and it will re-run `make run_all` whenever arith.cpp or the Makefile changes.

I've personally found that this is an extremely useful tool for rapid development, as my default environment is sublime + commandline.

## Extra Arith feature:

I implemented ternary if. My implementation is pretty clunky and horrible (5 arguments, ugh), but this adds significant utility to this toy language: I'm pretty sure that it's still not turing complete, but branching makes it technically capable of calculating -most- things (within the limitations of the 32-bit integer arithmetic available in c++ templates anyways, haha).

Usage is

    If< Expr1, Expr2, Less, Equal, Greater >

This evaluates Expr1 and Expr2, and then selects and executes either Less, Equal, or Greater depending on the relationship between Expr1 and Expr2.

Alternatively, division would be quite useful, as that's somewhat non-trivial to implement even with just branching. That wouldn't be very interesting to implement however, so instead I went with a form of branching.

## More info:

An explanation of how c++ template metaprogramming works is probably beyond the scope of this readme, but for starters you could check out Andrei Alexandrescu's excellent "Modern C++". And I do have a hard copy of it if anyone wants to borrow it; it's an excellent, albeit sadly obsolete treatise on some of the brilliant things that you could do with c++ template programming, well before features in modern c++ were available. It's also probably responsible for the creation of boost, which could probably be viewed as a good or bad thing...

Note: the techniques used here aren't really specific to modern c++ (ie. c++11/14/17) at all, and would probably work just as well with c++03.

Obviously, with modern c++ evaluating things at compile time is as simple as declaring a function constexpr (and then following a few rules / limitations), but doing that is nowhere near as interesting as this :)

The c++ type system (ie. templates) is technically turing complete, and bears quite a lot of similarity to haskell / other ML languages. It's quite interesting, albeit not very efficient as a programming language, haha.
