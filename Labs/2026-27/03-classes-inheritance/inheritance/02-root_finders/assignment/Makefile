# Usage: make        (build)
#        make run    (build and run)
#        make clean / make distclean

CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -g
# AddressSanitizer is on: as we saw in Lab 2, always use it while developing
CXXFLAGS += -fsanitize=address
LDFLAGS  += -fsanitize=address

EXE  = root_finders
OBJS = root_finders.o main.o

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $(EXE)

%.o: %.cpp root_finders.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(EXE)
	./$(EXE)

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) $(EXE)

.PHONY: all run clean distclean
