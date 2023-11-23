# Exercise 1 - MPI common comunications with custom size
Given two vectors $\mathbf a, \mathbf b \in \mathbb R^n$ write a function using MPI to compute in parallel the inner product of the two vectors

$$\langle \mathbf a, \mathbf b \rangle = \sum_{i=0}^n a_i b_i$$

Assume the two vectors are only present in memory of process with rank 0. Use collective communications to scatter the vectors into chunk and gather the local results. Each chunk should have size `n / size + (rank % size > 0 ? 1 : 0)` which is equal to `n / size + (rank % size > 0)`.

**Suggestion:**  Start from the implementation of Exercise 3 in Laboratory 5 and substitute the `MPI_Scatter` with `MPI_Scatterv`.

# Exercise 2 - OpenMP Hello World
## Part 1
Write an 'Hello world' program using OpenMP where each thread greets the world specifying its id.

* Use the tag `#pragma omp parallel` to parallelize your code
* Remember to include the proper header and use the compile flag `-fopenmp`
* Use the function `omp_get_thread_num()` to get the the local id

## Part 2
Modify the previous code to add the following characteristics:

* Use a private variable for each thread to save the thread id. Hint: use the syntax `private(thread_id)` just after the `#pragma omp parallel` tag.
* Use a shared variable for keeping in memory the total number of threads. Hint: use the syntax `shared(n_threads)` and the function `omp_get_num_threads()`
* Use `#pragma omp critical` and let one thread execute the greeting block at a time.
* Wait for all threads to complete and make one thread output the total number of threads in use. Hint: use `#pragma omp barrier` to wait for all threads, use `#pragma omp single` to execute a block with **only** the first thread available.

## Warning
We strongly suggest to use the compile flag `-Wall` when using OpenMP to show warnings related to the `#pragma` pre-processor commands. For instance, it is easy to forget the flag `-fopenmp` or mistype a pre-provessor command: if you do not put `-Wall` the compiler may not warn you about any of these issues.

# Exercise 3 - Matrix-Matrix Multiplication
The code in `serial.cpp` performs a matrix-matrix multiplication. Parallelize it using OpenMP directives. Remember to include Eigen during compilation.

```bash
g++ solution.cpp -O3 -I${mkEigenInc} -Wall -fopenmp
```

# Exercise 4 - MPI-OpenMP hybrid

## How Does Hybridization Help?
Hybrid application programs using MPI + OpenMP are now commonplace on large HPC systems. There are essentially two main motivations for this combination of programming models: 
* introducing MPI into OpenMP applications can help scale across multiple shared-memory multiprocessing (SMP) nodes;
* introducing OpenMP into MPI applications can help make more efficient use of the shared memory on each individual SMP node, thus mitigating the need for explicit intra-node communication;

## Assignment
Implement the code to integrate numerically a function $f: [0, 1] \rightarrow \mathbb R$ using the Simpson quadrature method using MPI and OpenMP. Employ the following strategy:
* Read from the arguments of the executable `argv` the number of intervals $n$ used for the numerical integration and the number `nthreads` of OpenMP threads the user intends to use.
* Partition the interval $[0, 1]$ into `size` sub-intervals $[a_i, b_i], i = 1, ...,$ `size` so that $a_1 = 0, b_i = a_{i+1},  b_{size}=1$. 
* Each MPI processor numerically integrates $f$ on one of these sub-intervals using a multithreaded function that parallelizes the Simpsons quadrature formula using `nthreads` OpenMP threads.
* The total integral will be the sum of the integrals on the partitions.

## Running the program
Since our PC is equivalent to one SMP node, we cannot really test our program on our PC. However, we can use the `-map-by node:PE=<n-threads-per-node>` flag to give to each MPI proc more than one cpu. In this way, each proc simulates one SMP node.
