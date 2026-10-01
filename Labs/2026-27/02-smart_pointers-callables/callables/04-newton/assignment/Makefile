# Usage: make        (build)
#        make run    (build and run)
#        make clean

CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -O2

EXE = newton

all: $(EXE)

# newton.hpp contains templates only: there is a single source file
$(EXE): main.cpp newton.hpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(EXE)

run: $(EXE)
	./$(EXE)

clean:
	$(RM) $(EXE)

.PHONY: all run clean
