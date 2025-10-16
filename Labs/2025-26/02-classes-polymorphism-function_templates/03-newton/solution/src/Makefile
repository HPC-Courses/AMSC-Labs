CXX = g++
CXXFLAGS = -std=c++20 -O2
CPPFLAGS = -I../include
PROGRAM = main
SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:%.cpp=%.o)
HEADERS=../include/$(wildcard *.h)
.PHONY : all clean distclean

.DEFAULT_GOAL = all

all : $(PROGRAM)

$(PROGRAM) : $(OBJS)
	$(CXX) $(OBJS) -o $(PROGRAM)

clean :
	$(RM) $(OBJS)

distclean : clean
	$(RM) $(PROGRAM)
