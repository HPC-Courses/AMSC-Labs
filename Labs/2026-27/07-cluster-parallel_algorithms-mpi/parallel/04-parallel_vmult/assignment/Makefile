# Usage: make            (build both executables)
#        make run        (build with AddressSanitizer and run the tests)
#        make benchmark  (build optimised and run the tests and the benchmark)
#        make clean

CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++20
# While developing: debug information and AddressSanitizer, as in Lab 2
DEBUG_FLAGS   = -O0 -g -fsanitize=address
# For the timings: optimised, without sanitizer
RELEASE_FLAGS = -O3
# The parallel algorithms of g++ use the library TBB
LDLIBS = -ltbb

# Only templates: the headers contain all the code, there is a single source file
HEADERS = sparse_matrix.hpp map_matrix.hpp csr_matrix.hpp

all: main_debug main

main_debug: main.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) main.cpp -o $@ $(LDLIBS)

main: main.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(RELEASE_FLAGS) main.cpp -o $@ $(LDLIBS)

run: main_debug
	./main_debug

benchmark: main
	./main benchmark

clean:
	$(RM) main main_debug

.PHONY: all run benchmark clean
