# Usage: make        (build)
#        make run    (build and run on shapes.txt)
#        make clean / make distclean

CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -g
# AddressSanitizer is on: as we saw in Lab 2, always use it while developing
CXXFLAGS += -fsanitize=address
LDFLAGS  += -fsanitize=address

EXE  = shapes
OBJS = shapes.o main.o

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $(EXE)

%.o: %.cpp shapes.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(EXE)
	./$(EXE) shapes.txt

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) $(EXE)

.PHONY: all run clean distclean
