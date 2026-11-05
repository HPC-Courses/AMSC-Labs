# Usage: make        (build the executable)
#        make run    (build and run)
#        make clean

CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -O2

# Only templates: the header contains all the code, there is a single source file
main: main.cpp derivatives.hpp
	$(CXX) $(CXXFLAGS) main.cpp -o $@

run: main
	./main

clean:
	$(RM) main

.PHONY: run clean
