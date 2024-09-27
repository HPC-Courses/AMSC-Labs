CXX = g++
OPENMPFLAG = #-fopenmp
OPENMPLINKINGFLAG =
LIBS = #-Linclude/lis-2.0.32/BUILD/lib -llis
CXXFLAGS = -std=c++20 -O2 -fpic  #-g
CPPFLAGS = -I../include #-DNDEBUG
PROGRAM = main
OBJ = main.o exe.o



.PHONY : all clean distclean

all : main


main : $(OBJ)
	$(CXX) $(OPENMPLINKINGFLAG) $(LIBS) $(OBJ) $(OPENMPFLAG) -o main.exe


$(OBJ) : %.o : %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(OPENMPFLAG) -c $<



clean :
	$(RM) *.o

distclean : clean
	$(RM) main.exe