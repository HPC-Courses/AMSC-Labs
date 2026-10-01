# Usage: make        (build)
#        make run    (build and run on datasets.txt)
#        make clean / make distclean

CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -g
# Uncomment to enable AddressSanitizer
# CXXFLAGS += -fsanitize=address
# LDFLAGS  += -fsanitize=address

EXE  = datasets
OBJS = datasets.o main.o

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $(EXE)

%.o: %.cpp datasets.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(EXE)
	./$(EXE) datasets.txt

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) $(EXE)

.PHONY: all run clean distclean
