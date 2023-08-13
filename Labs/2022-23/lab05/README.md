# Exerice 1 - Inner product

Given two vectors $\mathbf a, \mathbf b \in \mathbb R^n$ write a function using MPI to compute in parallel the inner product of the two vectors

$$\langle \mathbf a, \mathbf b \rangle = \sum_{i=0}^n a_i b_i$$

Assume the two vectors are only present in memory of process with rank 0. Use collective communications to scatter the vectors into chunk and gather the local results. You can exploit `std::inner_product` to compute the inner product on the local chunks.

# Exercise 2 - Power method
Implement with MPI a parallel function to approximate the dominant eigenvector of a square matrix $A$ via the power method.
Recall that the power method is an iterative algorithm. Let us assume that the matrix $A \in \mathbb R^{n \times n}$ has a dominant eigenvalue $\lambda$ with corresponding dominant eigenvector $\mathbf v \in \mathbb R^n$. If we choose an initial nonzero approximation $\mathbf v^{(0)} \in \mathbb R^n$ of one of the dominant eigenvectors of $A$ and we form the sequence given by:

$$\mathbf v^{(1)} =A \mathbf v^{(0)} / ||A \mathbf v^{(0)}||$$
$$\mathbf v^{(2)} =A \mathbf v^{(1)} / ||A \mathbf v^{(1)}||$$
$$...$$
$$\mathbf v^{(k)} =A \mathbf v^{(k-1)} / ||A \mathbf v^{(k-1)}||$$

if we assume that $A$ has an eigenvalue that is strictly greater in magnitude than its other eigenvalues and the starting vector $\mathbf v^{(0)}$ has a nonzero component in the direction of an eigenvector associated with the dominant eigenvalue, then a subsequence $\mathbf v^{(k)}$ converges to an eigenvector associated with the dominant eigenvalue. Moreover, under the two assumptions listed above, the sequence $\lambda^{(k)}$ defined by

$$\lambda^{(k)} = \frac{(\mathbf{v}^{(k)})^T A \mathbf v^{(k)}}{(\mathbf v^{(k)})^T \mathbf v^{(k)}} $$

converges to the dominant eigenvalue $\lambda$. For matrix operations and implementation, you can rely on all the methods of the `Eigen` library, whose
header file is provided in `#include <Eigen/Eigen>`. To compile use:

```bash
mpicxx main.cpp -O3 -I ${mkEigenInc} -Wall -pedantic -Wextra -std=c++20
```

To compute the normalization, you can use the Eigen method `norm()` even if it just works in serial. Note that matrices are stored row major in contiguous memory even if usually Eigen stores them in col major (see line 11 of `hint.cpp`). The input matrix $A$ and the initial guess $\mathbf v^{(0)}$ are available only on rank 0 process. For the sake of simplicity, assume that the matrix size evenly divides the number of processes. Stop the iterative process if we reach the maximum number of iterations or $||\mathbf v^{(k)}- \mathbf v^{(k-1)} ||$ is smaller than a given tolerance. In particular provide the implementation of the function `void power_method(const Matrix &A, Eigen::VectorXd &b, size_t max_iter, double toll)`.

# Exercise 3 - Sieve of Eratosthenes
The sieve of Eratosthenes is an ancient algorithm for finding all prime numbers up to any given limit. It does so by iteratively marking as composite (i.e., not prime) the multiples of each prime, starting with the first prime number, 2.

To find all the prime numbers less than or equal to a given integer $n$ by Eratosthenes' method:
  1. Create a list of consecutive integers from 2 through $n$: (2, 3, 4, ..., $n$).
  2.  Initially, let $p$ equal 2, the smallest prime number.
  3.  Enumerate the multiples of $p$ by counting in increments of $p$ from $2p$ to $n$, and mark them in the list. These will be $2p$, $3p$, $4p$, ...; the $p$ itself should not be marked. Actually, we just need to start marking numbers from $p^2$ since, if $q < p$ then $pq$ was already marked not prime at a previous iteration when $p=q$
  4. Find the smallest number in the list greater than $p$ that is not marked. If there was no such number, stop. Otherwise, let $p$ now equal this new number (which is the next prime), and repeat from step 3.
  5. When the algorithm terminates, the numbers remaining not marked in the list are all the primes below $n$.

## Segmented sieve
The problem with the sieve of Eratosthenes is not the number of operations it performs but rather its memory requirements. For large $n$, the range of primes may not fit in memory; worse, even for moderate $n$, its cache use is highly suboptimal. The algorithm walks through the entire array A, exhibiting almost no locality of reference. 

A solution to these problems is offered by segmented sieves, where only portions of the range are sieved at a time:
   1. Divide the range (2, ..., $n$) into segments of some size $\Delta \leq \sqrt{n}$.
   2. Find the primes in the first (i.e. the lowest) segment, using the regular sieve.
   3. For each of the following segments, in increasing order, with $m$ being the segment's topmost value, find the primes in it as follows:
        * Set up a Boolean array of size $\Delta$, and
        * Mark as non-prime the positions in the array corresponding to the multiples of each prime $p \leq \sqrt{m}$ found so far, by calculating the lowest multiple of $p$ between $m - \Delta$ and $m$, and enumerating its multiples in steps of $p$ as usual. The remaining positions correspond to the primes in the segment, since the square of a prime in the segment is not in the segment (for $k \geq 1$, one has $(k\Delta +1)^{2}>(k+1)\Delta$).

## Implementation details
In `serial.cpp` you will find 5 different implementations of the sieve that exploit a combination of the following three tricks:
1. The consumed memory is a bottleneck for big  $n$ . One simple way to save some memory is to use `std::vector<bool>`, which is a special implementation of vector that uses just one bit for each bool. This is an x8 save in terms of memory compared to using `char`.
2. The position of even number is easily predictable, then we can save in our `std::vector<bool> is_prime` just the position of non-even numbers starting from 3. Namely, the flag for the primeness of the **odd** number $m$ will be stored at index $(m - 3) / 2$.
3. Using a segmented sieve with partitions with the same size of the L1 cache of your processor. You can use `cat /proc/cpuinfo | grep -i cache` to check the size of the cache.

## Parallelization
We can exploit the segmented sieve to parallelize the code. Indeed, we just need to assign to each processor one of the segments of the sieve. Implement with MPI the parallel function `std::vector<char> get_primes(unsigned long n)` that given $n$ computes all the prime numbers smaller or equal to $n$. Assume that $n$ is known only by the rank 0 proc, moreover the rank 0 proc must collect all the prime numbers from the segments of the other processors.

