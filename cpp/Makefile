CXXFLAGS = -std=c++14 -Wall

default: run_all
all: no_output with_printf with_iostream
run_all: run_no_output run_printf run_iostream

no_output: build/arith_no_output build/arith_no_output.s
with_printf: build/arith_printf build/arith_printf.s
with_iostream: build/arith_iostream build/arith_iostream.s
with_error: build/arith_error build/arith_error.s

run_no_output: no_output
	./build/arith_no_output

run_printf: with_printf
	./build/arith_printf

run_iostream: with_iostream
	./build/arith_iostream

run_error: with_error
	./build/arith_error

build/arith_no_output: src/arith.cpp build
	$(CXX) $(CXXFLAGS) -o $@ $< -DNO_OUTPUT

build/arith_no_output.s: src/arith.cpp build
	$(CXX) $(CXXFLAGS) $< -DNO_OUTPUT -E > $@

build/arith_printf: src/arith.cpp build
	$(CXX) $(CXXFLAGS) -o $@ $< -DUSE_PRINTF

build/arith_printf.s: src/arith.cpp build
	$(CXX) $(CXXFLAGS) $< -DUSE_PRINTF -E > $@

build/arith_iostream: src/arith.cpp build
	$(CXX) $(CXXFLAGS) -o $@ $< -DUSE_IOSTREAM

build/arith_iostream.s: src/arith.cpp build
	$(CXX) $(CXXFLAGS) $< -DUSE_IOSTREAM -E > $@

build/arith_error: src/arith.cpp build
	$(CXX) $(CXXFLAGS) -o $@ $< -DINDUCE_ERROR

build/arith_error.s: src/arith.cpp build
	$(CXX) $(CXXFLAGS) $< -DINDUCE_ERROR -E > $@

interactive:
	when-changed Makefile src/*.cpp -c "clear; make run_all"

build:
	mkdir build

clean:
	rm -rf build
