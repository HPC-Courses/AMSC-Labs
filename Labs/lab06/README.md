# Exercise 1 - OpenMP Hello World
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

# Exercise 2 - Matrix-Matrix Multiplication
The code in `serial.cpp` performs a matrix-matrix multiplication. Parallelize it using OpenMP directives. Remember to include Eigen during compilation.
```g++ solution.cpp -O3 -I ${mkEigenInc} -Wall -fopenmp```

# Exercise 3 - MPI-OpenMP hybrid

## How Does Hybridization Help?
Hybrid application programs using MPI + OpenMP are now commonplace on large HPC systems. There are essentially two main motivations for this combination of programming models: 
* introducing MPI into OpenMP applications can help scale across multiple SMP nodes;
* introducing OpenMP into MPI applications can help make more efficient use of the shared memory on SMP nodes, thus mitigating the need for explicit intra-node communication;

## Assigment
Implement the code to integrate numerically a function $f: [0, 1] \rightarrow \mathbb R$ using the Simpson quadrature method using MPI and OpenMP. Employ the following strategy:
* Read from the arguments of the executable `argv` the number of intervals $n$ used for the numerical integration and the number `nthreads` of OpenMP threads the user intends to use.
* Partition the interval $[0, 1]$ into `size` sub-intervals $[a_i, b_i], i = 1, ...,$ `size` so that $a_1 = 0, b_i = a_{i+1},  b_{size}=1$. 
* Each MPI processor numerically integrates $f$ on one of these sub-intervals using a multithreaded function that parallelizes the Simpsons quadrature formula using `ntreads` OpenMP threads.
* The total integral will be the sum of the integrals on the partitions.

# Exercise 4 (Extra) - OpenMP tasks
`serial.cpp` provides a naive implementation of a function that builds a kd-tree. Parallelize it using OpenMP task.
