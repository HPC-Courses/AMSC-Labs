# A minimal Makefile for function-hello.
# Usage: make        (build)
#        make clean  (remove objects)
#        make distclean (remove objects and executable)

CXX      = g++
CXXFLAGS = -Wall -std=c++20 -I .

EXE  = hello
OBJS = hello.o main.o

# The first target is the default one: "make" is "make all"
all: $(EXE)

# Link: the executable depends on the object files
$(EXE): $(OBJS)
	$(CXX) $(OBJS) -o $(EXE)

# Compile: each object depends on its source and on the header
# $< is the first prerequisite (the .cpp), $@ is the target (the .o)
%.o: %.cpp hello.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) $(EXE)

# These targets are not files
.PHONY: all clean distclean
