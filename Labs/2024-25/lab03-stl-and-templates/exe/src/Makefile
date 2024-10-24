CXX = g++
OPENMPFLAG = #-fopenmp
OPENMPLINKINGFLAG =
LIBS = #-L/u/sw/toolchains/gcc-glibc/11.2.0/pkgs/lis/2.0.30/lib -llis
CXXFLAGS = -std=c++20 -O2 -fpic  #-g
CPPFLAGS = -I../include #-I/u/sw/toolchains/gcc-glibc/11.2.0/pkgs/lis/2.0.30/include #-DNDEBUG
PROGRAM = main
OBJ = main.o #utils.o
 


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
