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
  // FILL HERE

  // calculate the local segment size
  // FILL HERE

  // calculate the first number of the local segment
  // FILL HERE

  // initialize the `is_prime` local vector
  // FILL HERE

  // initialize the current number we are considering (to 2 since it is the first prime)
  // FILL HERE
  while (/*the square of the current prime is smaller than `n`*/) {
    // find the index of the first number that is multiple of the current prime in our partition
    // if the square of the current prime is greater than the start of the current segment 
    // we start directly from the square of the current prime, otherwise we have to check
    // the reminder of the first number of the local segment w.r.t. the current prime 
    // and consequently find which is the first multiple of the current prime
    unsigned long first_to_remove;
    // FILL HERE

    // apply the sieve, i.e. from first_to_remove to the end of the segment
    // flag as false `is_prime` each multiple of the current prime
    for (/*FILL HERE*/) {
      // FILL HERE
    }

    // just rank 0 search for the next prime
    // we are assuming that smallest prime greater than sqrt(n) belongs to rank 0
    // since usually `size` << `n`, this is reasonable
    // FILL HERE

    // broadcast the prime number we have found to all proc
    // FILL HERE
  }

  // allocate buffer to collect `is_prime` segments at rank 0
  // FILL HERE

  // calculate counts and displacements for MPI_Gatherv of `is_prime`
  // FILL HERE

  // do the MPI_Gatherv
  // FILL HERE
  
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