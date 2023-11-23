#include <mpi.h>

#include <array>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

// In this version, if a and b have size `n` that is not exactly divisible
// by the number of procs `size`, we split the vectors in chunks of size
// `n / size` plus 1 if the remainder `n % size` is greater than `rank`.
// In this way we have `size` chunks :
//  `n % size` chunks of size `n / size +1`
// and
//  `size - n % size` chunks of size `n /size`
// Since each chunk has a different size we have to employ `MPI_Scatterv` and
// specify where each chunk begins and which is its size.
double inner_product(const std::vector<double>& a,
                     const std::vector<double>& b) {
  if (a.size() != b.size()) {
    std::cerr << "Incompatible vector size" << std::endl;
    std::exit(-1);
  }

  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // broadcast the global size to prepare the local buffers
  int n = a.size();
  MPI_Bcast(&n, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
  // the local size is `n / size` plus 1 if the reminder `n % size` is greater
  // than `rank` in this way we split the load in the most equilibrate way
  const auto local_size = n / size + (n % size > rank);
  std::vector<double> local_a(local_size), local_b(local_size);

  // since each chunk has a different size we have to prepare buffers with
  // sizes and displacements of the chunk we have to send
  std::vector<int> sizes(size), displs(size + 1);
  for (int i = 0; i < size; ++i) {
    sizes[i] = n / size + (n % size > i);
    displs[i + 1] = displs[i] + sizes[i];
  }

  // scatter the chunks
  MPI_Scatterv(a.data(), sizes.data(), displs.data(), MPI_DOUBLE,
               local_a.data(), local_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Scatterv(b.data(), sizes.data(), displs.data(), MPI_DOUBLE,
               local_b.data(), local_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // compute local inner product
  const double partial = std::inner_product(local_a.cbegin(), local_a.cend(),
                                            local_b.cbegin(), 0.0);
  // sum togheter all local inner products
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