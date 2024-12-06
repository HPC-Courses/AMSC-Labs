# Exercise 1 - argc and argv, I/O synchronization
The aim of this exercise is to implement a slightly more advanced version of the standard MPI "Hello World" example. Namely it will have two features:
* The I/O is handled only by the processor with rank zero. Namely, only processor zero will use `std::cout`, the other processors will need to send the message they want to output to processor zero.
* The "Hello world" greeting is personalized by looking at the arguments passed to the executable at run-time. Namely, if at least one extra argument is provided, it will be who we greet (e.g. "Hello Matteo"). If, on the other hand, no argument is provided, the standard "Hello world" is printed.

Compile your executable with the following flags:
```
mpicxx main.cpp -Wall -Wextra -pedantic -std=c++20 -O0
```
Test it with no extra arguments
```
$ mpirun -n 2 ./a.out
Hello world, from rank 0 of 2.
Hello world, from rank 1 of 2.
```
and with one extra argument
```
$ mpirun -n 2 ./a.out Matteo
Hello Matteo, from rank 0 of 2.
Hello Matteo, from rank 1 of 2.
```
Notice that in this way we are guaranteed that the greeting will be printed without superimposition between processors and with increasing rank values.

# Exercise 2 - $N$-dimensional gradient computation
Given the $N$-dimensional scalar field $f$

$$f: \mathbb R^N \rightarrow \mathbb R,$$

we want to compute its gradient at a given point $\mathbf y \in \mathbb R^N$

$$\nabla f(\mathbf x) |_{\mathbf y } = 
\begin{bmatrix}
\partial_{x_1}f(\mathbf x)|_{\mathbf y} \\\ \vdots \\\ \partial_{x_N}f(\mathbf x)|_{\mathbf y}
\end{bmatrix}
.$$

To this end, we employ centered finite differences to approximate each component of the gradient. Namely:

$$\partial_{x_i}f(\mathbf x)|_{\mathbf y} \approx \frac{f(\mathbf y^+_i) - f(\mathbf y^-_i)}{2h},$$

where $\mathbf y^+_i, \mathbf y^-_i \in \mathbb R^N$ are vectors equal to $\mathbf y$ apart from the $i$-th component, namely

$$\mathbf y^+_i = 
\begin{bmatrix}
y_0 \\\ \vdots \\\ y_{i-1} \\\ y_i + h \\\ y_{i+1} \\\ \vdots \\\ y_N
\end{bmatrix},
\quad
\mathbf y^-_i =
\begin{bmatrix}
y_0 \\\ \vdots \\\ y_{i-1} \\\ y_i - h \\\ y_{i+1} \\\ \vdots \\\ y_N
\end{bmatrix}.$$

Implement a function (that should work for any $N$)
```cpp
template<size_t N>
std::array<double, N> compute_gradient(
  const std::function<double (const std::array<double, N> &)> &f,
  const std::array<double, N> &y, 
  double h)
```
that given as inputs:
* the scalar field $f$, `const std::function<double (const std::array<double, N> &)> &f`
* the point $\mathbf y$, `const std::array<double, N> &y`
* the finite difference step $h$, `double h`

returns the gradient $\nabla f(\mathbf x) |_{\mathbf y }$ as `std::array<double, N>`, where each component is computed in parallel by a different processor and is then collected by processor zero by means of `MPI_Send` and `MPI_Recv`. We assume that $\mathbf y, h$ and $N$ are already known by all processors.

# Exerice 3 - Inner product

Given two vectors $\mathbf a, \mathbf b \in \mathbb R^n$ write a function using MPI to compute in parallel the inner product of the two vectors

$$\langle \mathbf a, \mathbf b \rangle = \sum_{i=0}^n a_i b_i$$

Assume the two vectors are only present in memory of process with rank 0. Use collective communications to scatter the vectors into chunk and gather the local results. 

**Suggestion:**  You can use `std::inner_product` to compute the inner product of the local chunks on each processor.