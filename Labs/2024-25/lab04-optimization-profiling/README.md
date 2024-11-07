# Profiling and optimizing


## Valgrind
Valgrind is a GPL'd system for debugging and profiling Linux programs. With Valgrind's tool suite you can automatically detect many memory management and threading bugs, avoiding hours of frustrating bug-hunting, making your programs more stable. You can also perform detailed profiling to help speed up your programs. The Valgrind distribution includes [many debugging and profiling tools](https://valgrind.org/info/tools.html), we will concentrate on two: 

- Memcheck to detect memory-management problems
```bash
valgrind --tool=memcheck ./main
```

- Callgrind which performs detailed simulation of the caches in your CPU plus extra information about callgraphs.

```bash
valgrind --tool=callgrind --dump-instr=yes --collect-jumps=yes ./main
```

To visualize (in a nice graphical way) the results of the profiling done with Valgrind, we use KCacheGrind. You can obtain it in the following ways:

- Linux or [WSL with GUI](https://learn.microsoft.com/en-us/windows/wsl/tutorials/gui-apps) (graphical interface inside WSL): run `sudo apt-get install kcachegrind`
- Windows (on the host machine): install [QCacheGrind port for windows](https://sourceforge.net/projects/qcachegrindwin/) 
- MacOs (on the host machine): run the following commands `brew update`, `brew install qcachegrind`

## lcov
In software engineering, code coverage is a percentage measure of the degree to which the source code of a program is executed when a particular test suite is run. A program with high test coverage has more of its source code executed during testing, which suggests it has a lower chance of containing undetected software bugs compared to a program with low test coverage. Moreover, it can give you an hint of where there may be potential bottolnecks or which parts of your code you should optimize more carefully since are executed many times.

To run the code coverage with `lcov` you need the following steps

1. Compile your code with proper compiler flags `g++ main.cpp -O0 -g -o main --coverage`
2. Create the files for saving the coverage and reset the statistic if present `lcov --directory . --zerocounters`
3. Run your executable `./main`
4. Create the coverage output `lcov --directory . --capture --output test_coverage.info`
5. Create a nice website to visualize the results `genhtml test_coverage.info --output test_coverage`
6. Open the `test_coverage/index.html` file, which is the root of the website


## Exercise - profile and optimize the given code
You are given a code in `start.cpp` that instanciate at random some triangles in a 3D space and then, for each one of them, compute the normal vector. Each triangle has also a color in RGB and a transparency value. Your task is to make this code as fast as possible.

### Step 1 - minimize heap allocations
One of the first things that you disturb you about this code is that when running Memcheck you see a lot of allocations. As we know, allocations of the heap are expensive and, sometimes, non sequential in physical memory, making the job of the CPU harder. The first suggestion is then to modify the data structures to use static memory as much as possible.

### Step 2 - do not use polymorphism if you are not exploiting it
If in our code we are using only triangles it is useless to keep an abstract base class and the overhead given by using a virtual method.

### Step 3 - minimize data padding
Data structure alignment is the way data is arranged and accessed in computer memory.

The CPU in modern computer hardware performs reads and writes to memory most efficiently when the data is naturally aligned, which generally means that the data's memory address is a multiple of the data size. For instance, in a 32-bit architecture, the data may be aligned if the data is stored in four consecutive bytes and the first byte lies on a 4-byte boundary.

Data alignment is the aligning of elements according to their natural alignment. To ensure natural alignment, it may be necessary to insert some padding between structure elements or after the last element of a structure. For example, on a 32-bit machine, a data structure containing a 16-bit value followed by a 32-bit value could have 16 bits of padding between the 16-bit value and the 32-bit value to align the 32-bit value on a 32-bit boundary. Alternatively, one can pack the structure, omitting the padding, which may lead to slower access, but uses three quarters as much memory. 

```cpp
/// Layout:
/// | c | padding | padding | padding | // "i" would not fit in this row!
/// | i |    i    |    i    |    i    |
/// | s |    s    | padding | padding |
struct Struct {
  char c;       // 1 byte.
  int i;        // 4 bytes.
  short int s;  // 2 bytes.
};
```

With this knowledge try to minimize the padding in the `Triangle` class. *Suggestion:* in these cases `sizeof` is your best friend.

### Step 4 - compiler flags
The final step consist in adding suitable compiler flags for out task, here a list:
 - `-O3` Highest level of standard-compliant optimization. See also [https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html).
 - `-Ofast` Optimize while disregarding strict standards compliance. Enables all `-O3` optimizations. It also enables optimizations that are not valid for all standard-compliant programs, like `-ffast-math`.
 - `-march=native` tune generated code for the micro-architecture and ISA extensions of the host CPU. **Warning**: it is a destructive flag: it makes the binary possible not compatible on a lot of hardware.
 - `-funroll-all-loops` the optimizer determines and applies the best unrolling factor for each loop; in some cases, the loop control might be modified to avoid unnecessary branching. 


# Homework
In the folder `homework` you are provided with a simple program composed by a source file with the `main` function and a header file where a list data structure is implemented. The code has several errors, try to find and fix them. 

*Hint:* there are three kind of errors:
1. **Compile-time error**: errors that appear when you try to compile the code and are caused by a wrong sintax
2. **Run-time error**: errors that will make your executable go in segmentation fault when you try to run it
3. **Memory leak**: on the surface the code will look like as everything is fine, but under the hood there is a memory leak. To check for its presence you can use the `--tool=memcheck` option of Valgrind

# Extra - More on struct memory alignment
In the folder `extra-padding` we provide a more advanced example on struct memory alignment where you can see in action the `#pragma pack` preprocessor directive and how it affects to memory alignment. In particular, we provide a measure of the following statistics:
- Size in memory
- Speed of allocation
- Speed of write
- Speed of read

We suggest to compile the code with 
```bash
g++ main.cpp -Wall -Wextra -O3 -o main 
```
