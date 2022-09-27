# Exercise 1 - Static and shared libraries
Often in your code you will need to employ libraries developed by others. They come in two flavors: static libraries and shared libraries.

Shared libraries are `.so` files. All the code relating to the library is in this file, and it is referenced by programs using it at run-time. A program using a shared library only makes reference to the code that it uses in the shared library.

Static libraries are `.a` files. All the code relating to the library is in this file, and it is directly linked into the program at compile time. A program using a static library takes copies of the code that it uses from the static library and makes it part of the program.

There are advantages and disadvantages in each method:

- Shared libraries reduce the amount of code that is duplicated in each program that makes use of the library, keeping the binaries small. It also allows you to replace the shared object with one that is functionally equivalent, but may have added performance benefits without needing to recompile the program that makes use of it. Shared libraries will, however have a small additional cost for the execution of the functions as well as a run-time loading cost as all the symbols in the library need to be connected to the things they use. Additionally, shared libraries can be loaded into an application at run-time, which is the general mechanism for implementing binary plug-in systems.

- Static libraries increase the overall size of the binary, but it means that you don't need to carry along a copy of the library that is being used. As the code is connected at compile time there are not any additional run-time loading costs. The code is simply there.

## How to build a shared library?
We will dedicate another lecture to this issue, where we will also show how to handle shared libraries and symbols dynamically. For the moment we need to know only the following:
- When building a shared library we need to pass the option `-shared` to the linker
- Object code used in a shared library must be position independent (compiler option `-fPIC`)
Basic build of a shared library starting from an object file:
```bash
g++ -shared -Wl,-soname,libutility.so utility.o -o libutility.so
```

## Where does the loader search for shared libraries?
In computer systems a loader is the part of an operating system that is responsible for loading programs and libraries. It is one of the essential stages in the process of starting a program, as it places programs into memory and prepares them for execution. Loading a program involves reading the contents of the executable file containing the program instructions into memory, and then carrying out other required preparatory tasks to prepare the executable for running. This includes finding the libraries linked in the executable.

It looks in `/lib`, `/usr/lib`, in all the directories contained in `/etc/ld.conf` and in all `.conf` contained in the `/etc/ld.conf.d/` directory (so the search strategy is different than that of the linker!.

Alternative ways of directing the loader are:
- Setting the environment variable `LD_LIBRARY_PATH`. If it contains a comma-separated list of directory names the loader will first look for libraries on these directories (analogous to PATH for executables):
```bash
export LD_LIBRARY_PATH+=:dir1:dir2
```
- With the special flag `-Wl,-rpath=directory` during the compilation of the executable, for instance
```bash
g++ main.cpp -o main -Wl,-rpath=/opt/lib -L. -lsmall
```
Here the loader will look in `/opt/lib` before the standard directories. You can use also relative paths.

## Listing the shared libraries used
The command `ldd` lists the shared libraries used by an object file.
For example:
```bash
module load octave
ldd ${mkOctavePrefix}/lib/octave/6.2.0/liboctave.so
```
It means that the version of Octave I have has been linked (by its developers) against version 3 of the `libfftw3` library.

## Assignements
1. Compile `test.cpp` including the library `mylib.hpp` directly.
2. Compile `mylib.hpp` as a shared library.
3. Compile `test.cpp` linking the shared library `mylib.so`.
4. Use `ldd` on the two executable of the main, what are the differences?
5. Compile `mylib.hpp` as a shared library without using the flag `-Wl,-rpath=directory`.

## Solutions
<details>
<summary>Spoiler solutions</summary>

1. `g++ -I ${mkEigenInc} -I ./mylib/ main.cpp -o main_embedded`
2. `cd mylib/ && g++ -fPIC -shared -Wl,-soname,libmylib.so mylib.hpp -o libmylib.so`
3. `g++ -I ${mkEigenInc} -I ./mylib/ -L ./mylib -lmylib -Wl,-rpath=${PWD}/mylib main.cpp -o main_shared`
4. `ldd main_embedded`, `ldd main_shared`, in the first case there is not `mylib` among the shared libraries.
5. `export LD_LIBRARY_PATH+=:$PWD/mylib && g++ -I ${mkEigenInc} -I ./mylib/ -L ./mylib -lmylib main.cpp -o main_shared`

</details>


# Exercise 2 - Templates
Exponentiation by squaring is a general method for fast computation of large positive integer powers of a number or in general objects that have a multiplication operation (more precisely an element of a [semigroup](https://en.wikipedia.org/wiki/Semigroup), e.g. a matrix or a polynomial). 

Let's start with an example:
$$a^{13} = a^8 a^4 a^1, a \in \mathbb R$$
Thus given $a$ we can compute $r=a^{13}$ with just seven operations as follows
```
double r = 1.0;
r *= a; // r == a    on the rhs is intended as the original value of a
a *= a; // a == a^2
a *= a; // a == a^4
r *= a; // r == a^5
a *= a; // a == a^8
r *= a; // r == a^13
```

In general we have the following recursive formula:
$x^{n}=x\,(x^{2})^{\frac {n-1}{2}}$ if $n$ is odd, $x^{n}=(x^{2})^{\frac {n}{2}}$ if $n$ is even.


Implement a template function that given a base $x$ of type `typename T` and an exponent $n$ of type `unsigned int` computes $x^n$. 

# Exercise 3 - Newton's method
Newton's method is a numerical algorithm used to find roots of a function iteratively. If the function satisfies sufficient assumptions and the initial guess is close, then

$$x_{n+1}=x_{n}-{\frac {f(x_{n})}{f'(x_{n})}}$$

Write a `NewtonSolver` class that takes as constructor arguments 
- the function $f(x)$
- its derivative $f'(x)$
- the maximum number of iterations `max_iter`
- the tolerance on the residual `rtol`
- the tolerance on the step size `stol`

Has a method `solve` that takes as input the starting point $x_0$, and has the following members (accessible with a getter):
- `iter` number of iterations
- `res` residual
- `xs` the history of the iterations

Apply it to the function $f(x) = x^2 - 2$. Pass it as:
- a function pointer
- a lambda

## Advanced assignements
Use automatic differentiation to compute the detivative $f'(x)$. In particular, give the class
```cpp
using func_t = std::function<double(double)>;

class Var {
  public:
    Var(
      const func_t& val,
      const func_t& der
    ) :
      m_val(val), m_der(der) {}

    double eval(double x) const {
      return m_val(x);
    }
    double eval_der(double x) const {
      return m_der(x);
    }
private:
    func_t m_val, m_der;
}
```
implement appropriate overloaded operators to sum (`operator+`) and multiply (`operator*`) variables together. Follow the [standard convetions for operators](https://en.cppreference.com/w/cpp/language/operators). Adapt the class `NewtonSolver` to use a single `Var` instead of a pair of `std::function<double(double)>` for $f(x)$ and $f'(x)$.

# Exercise 4 - Horner's method
Horner's method is an algorithm for polynomial evaluation. The algorithm is based on Horner's rule:
$$a_{0}+a_{1}x+a_{2}x^{2}+a_{3}x^{3}+\cdots +a_{n}x^{n}$$
$$=a_{0}+x{\bigg (}a_{1}+x{\Big (}a_{2}+x{\big (}a_{3}+\cdots +x(a_{n-1}+x\,a_{n})\cdots {\big )}{\Big )}{\bigg )}.$$
1. Evaluate the complexity of the algorithm in terms of $n$, assume power elevation can be done only throught multiplicatoion
2. Implement the following functions:
    - `eval`: given a vector of the coefficient of a polynomial evaluates the polynomial at the given point $x$ with the naive formula
    - `eval_horner`: given a vector of the coefficient of a polynomial evaluates the polynomial at the given point $x$ with the horner's rule
    - `evaluate_poly`: given a vector of the coefficient of a polynomial and an evaluation function (i.e. either `eval` or `eval_horner`) evaluates the polynomial at each given point in a vector.
3. Using the standard library `<chrono>` evaluate the difference in time between the two methods for a polynomial of degree $n$, read from standard input, and with coefficients $2\sin(2k)$ at $M$ points between $x_0=0$ and $x_f=1$, where $M$ is hard coded.
4. Test the algorithms for varius values of $n$ and $M$.

## Advanced assignements
- Use the `<regex>` library to parse the file `params.dat` and make $n, M, x_0, x_f$ parametric. Use an appropriate data structure to store the parameters.
- Implement a funtion `timeit` that given a `void()` function times its exectution time using `<chrono>`. Define a vector of tests containing `eval` and `eval_horner`; use `timeit` to time their execution. 
- Use `<ranges>` to instanciate the coefficients of the polynomial and the evaluation points without using a for loop.
- Modify `evaluate_poly` to use `std::transform` instead of a for loop. Exploit the parallel execution policies of `std::transform`. **WARNING:** requires to link against the Intel Threading Building Blocks (TBB) library (preprocessor flags `-I${mkTbbInc}`, linker flags `-L${mkTbbLib} -ltbb`)
- Convert eval and eval horner from function pointers to `std::function`
- Use exponentiation by squaring in `eval`


# Exercise 5 - Branchless programming
`if`, `switch`, and other conditional statements often corresponds to machine instructions that can’t be effectively predicted by the CPU. This is due to the fact that they correspond to 'jump' instructions that make the CPU interrupt the flow of instruction to jump to another adress. Indeed, they muse a long pipeline stall to fetch new instructions after a branch mispredict.

Take for instance the function `smaller_standard` in `smaller.cpp`. This is perfectly fine programming but it **might** cause performance issues due to the `if` statement.

## How to diagnost problems?
The best way is benchmarking your code (i.e. check its run time in significant scenarios), however we have some tools that can help us discovering this king of problems without having to benchmark our code.

One of the best options is to check the Assembly code generate by your programm. To do this just use the flag `-S` when compiling. There are also interactive web tools that help you understand better the generate Assembly, see for instance [Compiler Explorer](https://godbolt.org/).

1. Compile `smaller.cpp` and check its machine code. Try different optimization options `-O0`, `-O1`, `-O2`, `-O3`. What do you see?
2. Repeat the same for `toupper.cpp`. What do you expect? Run the benchmark and see if you where right.

<details>
<summary>Spoiler solutions</summary>

1. `g++ smaller.cpp -S -O3`. The compiler actually saw what we were trying to do and found a branchless way to do that. It lead to something like the following: it compares `a` and `b`, moves `b` into the memory pointed by `eax` and conditionally moves `a` into `b` if it is less.
```
cmp     [a], [b]
mov     eax, [b]
cmovle  eax, [a]
```

2. `g++ toupper.cpp -S -O3`. In this case the branchless version is much faster and lead to smaller code without the `ja` instruction. Running the benchmark should result in a running time that is 50%-65% smaller.

</details>

# Exercise 6 - Copy elision
Under some circustances, thank to [copy elision](https://en.cppreference.com/w/cpp/language/copy_elision), the compiler can optimize away a copy in 
- a return statement 
- during the initialization of an object

Compile `main.cpp` and try to understand where copy elision is applied.
This is important because, among other things, it allows to write more readable code. Indeed, suppose you have a `HugeDynamicObject` that requires memory allocation on the heap. Suppose you want to write a function `parametrically_build_object` that creates `HugeDynamicObject` depending on some some parameters `p1, p2`. Without copy elision a funtion with the signature
```cpp
HugeDynamicObject parametrically_build_object(double p1, double p2) {
    //... some code
    return HugeDynamicObject{/**/};
}
HugeDynamicObject hdo = parametrically_build_object(1.0, 2.5);
```
would be very wastfull since it requires to copy the result of the function into `h`. It would be better something like:
```cpp
void parametrically_change_object(double p1, double p2, HugeDynamicObject &hdo) {
    //... modify hdo as you wish
}
HugeDynamicObject hdo;
parametrically_change_object(1.0, 2.5, hdo);
```
which is less readable, since you cannot immediatly understand which are the inputs and which are the outputs.