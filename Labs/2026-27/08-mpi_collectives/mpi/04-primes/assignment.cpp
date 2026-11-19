/*
The prime numbers below N. Each process looks for them in its block of
0, ..., N-1, and rank 0 collects them all, in order. Nobody knows in advance
how many primes each process finds.

compile with
  mpic++ -Wall -std=c++20 -O3 assignment.cpp -o assignment
and run it with
  mpirun -n 4 ./assignment
*/

#include <mpi.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

bool is_prime(int p) {
  if (p < 2)
    return false;
  for (int d = 2; d * d <= p; ++d)
    if (p % d == 0)
      return false;
  return true;
}

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  int size;
  int rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int N = 1000000;

  // The block of this process, first, ..., last - 1: the first N % size
  // processes get one number more, as in mpi/01-normalize
  const int first = rank * (N / size) + std::min(rank, N % size);
  const int last = first + N / size + (rank < N % size ? 1 : 0);

  std::vector<int> local_primes;
  for (int p = first; p < last; ++p)
    if (is_prime(p))
      local_primes.push_back(p);
  const int local_count = local_primes.size();
  std::cout << "rank " + std::to_string(rank) + ": " + std::to_string(local_count) +
                   " primes in " + std::to_string(first) + " ... " + std::to_string(last - 1) +
                   "\n";

  // TODO 1: rank 0 does not know how many primes each process has found.
  //         Collect the local_count of all the processes into counts, on
  //         rank 0, with MPI_Gather.
  std::vector<int> counts;

  // TODO 2: on rank 0, the displacements, and the size of primes
  std::vector<int> displacements;
  std::vector<int> primes;

  // TODO 3: rank 0 collects all the local_primes into primes, with MPI_Gatherv

  if (rank == 0 && !primes.empty()) {
    std::cout << primes.size() << " primes below " << N << ":";
    for (int i = 0; i < 5; ++i)
      std::cout << " " << primes[i];
    std::cout << " ...";
    for (std::size_t i = primes.size() - 3; i < primes.size(); ++i)
      std::cout << " " << primes[i];
    std::cout << std::endl;
  }

  MPI_Finalize();
  return 0;
}
