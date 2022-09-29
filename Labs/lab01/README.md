# Exercise 1 - Templates
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


Implement a template function that given a base $x$ of type `typename T` and an exponent $n$ of type `unsigned int` computes $x^n$. Start by implementing a recursive version, after that try implementing an iterative version.

# Exercise 2 - Horner's method
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


# Exercise 3 - Branchless programming
`if`, `switch`, and other conditional statements often corresponds to machine instructions that can’t be effectively predicted by the CPU. This is due to the fact that they correspond to 'jump' instructions that make the CPU interrupt the flow of instruction to jump to another adress. Indeed, they muse a long pipeline stall to fetch new instructions after a branch mispredict.

Take for instance the function `smaller_standard` in `smaller.cpp`. This is perfectly fine programming but it **might** cause performance issues due to the `if` statement.

## How to diagnose problems?
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