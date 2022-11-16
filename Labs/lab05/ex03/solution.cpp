#include <iostream>
#include <map>
#include <vector>

#include <mpi.h>

#include "../utils.hpp"

std::vector<char> get_primes(unsigned long n) {
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // broadcast n 
  MPI_Bcast(&n, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
  // calculate the local segment size
  const auto local_n = n / size + ((n % size) > (unsigned long)rank);
  // calculate the start of the segment
  const auto min = (n / size) * rank + std::min((unsigned long)rank, n % size);
  // initialize the `is_prime` vector
  std::vector<char> is_prime(local_n, '1');
  if (rank == 0) {
    is_prime[0] = is_prime[1] = '0';
  }

  // initialize the current number we are considering (to 2 since it is the first prime)
  unsigned long curr = 2;
  while (curr * curr < n) {
    // find the index of the first number that is multiple of the current prime in our partition
    unsigned long first_to_remove;
    if (curr * curr > min) {
      first_to_remove = curr * curr - min;
    } else {
      first_to_remove = (curr - (min % curr)) * ((min % curr) > 0);
    }
    // apply the sieve, i.e. from first_to_remove to the end of the segment
    // flag as false `is_prime` each multiple of the current prime
    for (unsigned long i = first_to_remove; i < local_n; i += curr) {
      is_prime[i] = '0';
    }
    // just rank 0 search for the next prime
    // we are assuming that smallest prime greater than sqrt(n) belongs to rank 0
    // since usually `size` << `n`, this is reasonable
    if (rank == 0) {
      while (!is_prime[curr++]) {};
    }
    // broadcast the prime number we have found to all proc
    MPI_Bcast(&curr, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
  }

  // allocate buffer to collect at rank 0
  std::vector<char> is_prime_global;
  // calculate counts and displacements for MPI_Gatherv
  std::vector<int> counts, displs;
  if (rank == 0) {
    is_prime_global.resize(n);
    displs.push_back(0);
    counts.push_back(local_n);
    for(int i = 1; i < size; ++i) {
      displs.push_back(counts.back() + displs.back());
      counts.push_back(n / size + (((int)n % size) > i));
    }
  }
  // do the MPI_Gatherv
  MPI_Gatherv(&is_prime[0], is_prime.size(), MPI_CHAR, &is_prime_global[0],
              &counts[0], &displs[0], MPI_CHAR, 0, MPI_COMM_WORLD);
  return is_prime_global;
}

int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<char> is_prime;
  const auto dt = timeit([&]() { is_prime = get_primes(100); });
  if (rank == 0) {
    std::cout << "Elapsed: " << dt << " [ms] " << std::endl;
    std::cout << is_prime << std::endl;
  }

  MPI_Finalize();
  return 0;
}