#include <mpi.h>

#include <array>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

// In this version, if a and b have size `n` that is not exactly divisible
// by the number of procs `size`, we split the vectors in chunks of size
// `n / size` and the remainder chunk of size `n % size` is handled by
// processor zero. In this way we have a load imbalance since proc 0 has
// to handle more computations. However this is an occasion to showcase the
// use of the plain `MPI_Scatter`
double inner_product(const std::vector<double>& a,
                        const std::vector<double>& b) {
  if (a.size() != b.size()) {
    std::cerr << "Incompatible vector size" << std::endl;
    std::exit(-1);
  }

  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // broadcast the local size to prepare the local buffers
  unsigned long local_size = a.size() / size;
  MPI_Bcast(&local_size, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
  // allocate buffers
  std::vector<double> local_a(local_size), local_b(local_size);

  // scatter data
  MPI_Scatter(a.data(), local_size, MPI_DOUBLE, local_a.data(), local_size,
              MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Scatter(b.data(), local_size, MPI_DOUBLE, local_b.data(), local_size,
              MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // compute the inner product of the reminder chuck `n % size` at the end of
  // `a` and `b`
  const double partial_reminder =
      rank ? 0.0
           : std::inner_product(a.cend() - a.size() % size, a.cend(),
                                b.cend() - b.size() % size, 0.0);
  // compute the inner product of the local chunk
  const double partial =
      partial_reminder + std::inner_product(local_a.cbegin(), local_a.cend(),
                                            local_b.cbegin(), 0.0);
  // the inner product is just the sum of the inner products of the local chunks
  double sum;
  MPI_Reduce(&partial, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  return sum;
}


// helper function that returns a vector of size N with entries 1, 2, ..., N
// only for proc 0
std::vector<double> get_test(size_t N) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> x;
  // simulate that the vector is owned only by the main proc
  if (rank == 0) {
    x.resize(N);
    std::iota(x.begin(), x.end(), 1);
  }
  return x;
}

int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double n = 8;
  // test for different sizes of vec
  for (size_t i = 0; i < 9; ++i) {
    if (rank == 0) {
      std::cout << "Allocating vector of size " << n << "..." << std::flush;
    }
    const std::vector<double> x = get_test(n);
    if (rank == 0) {
      std::cout << "done" << std::endl;
    }

    // call the function we have implemented and evaluate in how much time it
    // runs
    using namespace std::chrono;
    const auto t0 = high_resolution_clock::now();
    const auto xx = inner_product(x, x);
    const auto t1 = high_resolution_clock::now();
    const auto dt = duration_cast<milliseconds>(t1 - t0).count();
    // output results if this is proc 0
    if (rank == 0) {
      const double r = (n * (n + 1) * (2 * n + 1)) / 6.0;
      std::cout << "Elapsed: " << dt << " | Error: " << std::abs(xx - r) / r
                << std::endl;
    }
    n *= 10.;
  }

  MPI_Finalize();
  return 0;
}