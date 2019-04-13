CXXFLAGS = -std=c++14 -Wall

default: run_all
all: no_output with_printf with_iostream with_haskell
run_all: run_no_output run_printf run_iostream run_haskell

no_output: build/arith_no_output build/arith_no_output.s
with_printf: build/arith_printf build/arith_printf.s
with_iostream: build/arith_iostream build/arith_iostream.s
with_error: build/arith_error build/arith_error.s
with_haskell: build/arith_haskell build/arith_haskell.s

run_no_output: no_output
	./build/arith_no_output

run_printf: with_printf
	./build/arith_printf

run_iostream: with_iostream
	./build/arith_iostream

run_error: with_error
	./build/arith_error

run_haskell: with_haskell
	./build/arith_haskell

build/arith_no_output: src/arith.cpp build
	$(CXX) $(CXXFLAGS) -o $@ $< -DNO_OUTPUT

build/arith_no_output.s: src/arith.cpp build
	$(CXX) $(CXXFLAGS) -o $@ $< -S -DNO_OUTPUT

build/arith_printf: src/arith.cpp build
	$(CXX) $(CXXFLAGS) -o $@ $< -DUSE_PRINTF

build/arith_printf.s: src/arith.cpp build
	$(CXX) $(CXXFLAGS) -o $@ $< -S -DUSE_PRINTF

build/arith_iostream: src/arith.cpp build
	$(CXX) $(CXXFLAGS) -o $@ $< -DUSE_IOSTREAM

build/arith_iostream.s: src/arith.cpp build
	$(CXX) $(CXXFLAGS) -o $@ $< -S -DUSE_IOSTREAM

build/arith_error: src/arith.cpp build
	$(CXX) $(CXXFLAGS) -o $@ $< -DINDUCE_ERROR

build/arith_error.s: src/arith.cpp build
	$(CXX) $(CXXFLAGS) -o $@ $< -S -DINDUCE_ERROR

build/arith_haskell: src/arith.hs build
	ghc -o $@ $<

build/arith_haskell.s: src/arith.hs build
	ghc -o $@ $< -S && rm src/arith.hi src/arith.o

interactive:
	when-changed Makefile src/*.cpp -c "clear; make run_all"

build:
	mkdir build

clean:
	rm -rf build
