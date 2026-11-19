/*
The prime numbers below N. Each process looks for them in its block of
0, ..., N-1, and rank 0 collects them all, in order. Nobody knows in advance
how many primes each process finds. Solution.

compile with
  mpic++ -Wall -std=c++20 -O3 solution.cpp -o solution
and run it with
  mpirun -n 4 ./solution
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

  // 1. Rank 0 does not know how many primes each process has found: it
  //    receives them first, one int from each process.
  std::vector<int> counts;
  if (rank == 0)
    counts.resize(size);
  MPI_Gather(&local_count, 1, MPI_INT, counts.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

  // 2. Only rank 0 needs the displacements, and the room for all the primes
  std::vector<int> displacements;
  std::vector<int> primes;
  if (rank == 0) {
    displacements.resize(size, 0);
    for (int r = 1; r < size; ++r)
      displacements[r] = displacements[r - 1] + counts[r - 1];
    primes.resize(displacements[size - 1] + counts[size - 1]);
  }

  // 3. Each process sends its primes; rank 0 puts them one block after the
  //    other. The blocks are in order, so the primes are in order too.
  MPI_Gatherv(local_primes.data(), local_count, MPI_INT, primes.data(), counts.data(),
              displacements.data(), MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0) {
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
