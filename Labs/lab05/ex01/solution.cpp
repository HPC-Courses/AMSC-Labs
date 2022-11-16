#include <array>
#include <cmath>
#include <numeric>

#include <mpi.h>

#include "../utils.hpp"

// In this version, if a and b have size `n` that is not exactly divisible
// by the number of procs `size`, we split the vectors in chuncks of size
// `n / size` and the remainder chunck of size `n % size` is handled by 
// processor zero. In this way we have a load imbalance since proc 0 has 
// to handle more computations. However this is an occasion to showcase the 
// use of the plain `MPI_Scatter`
double inner_product_v1(const std::vector<double>& a,
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

  // compute the inner product of the reminder chuck `n % size` at the end of `a` and `b`
  const double partial_reminder =
      rank ? 0.0
           : std::inner_product(a.cend() - a.size() % size, a.cend(),
                                b.cend() - b.size() % size, 0.0);
  // compute the inner product of the local chunck
  const double partial =
      partial_reminder + std::inner_product(local_a.cbegin(), local_a.cend(),
                                            local_b.cbegin(), 0.0);
  // the inner product is just the sum of the inner products of the local chuncks
  double sum;
  MPI_Reduce(&partial, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  return sum;
}

// In this version, if a and b have size `n` that is not exactly divisible
// by the number of procs `size`, we split the vectors in chuncks of size
// `n / size` plus 1 if the remainder `n % size` is greater than `rank`.
// In this way we have `size` chuncks : * `n % size` chuncks of size `n / size + 1` and
//                                      * `size - n % size` chuncks of size `n / size`
// Since each chunck has a different size we have to employ `MPI_Scatterv` and specify
// where each chunck begins and which is its size.
double inner_product_v2(const std::vector<double>& a,
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
  // the local size is `n / size` plus 1 if the reminder `n % size` is greater than `rank`
  // in this way we split the load in the most equilibrate way
  const auto local_size = n / size + (n % size > rank);
  std::vector<double> local_a(local_size), local_b(local_size);

  // since each chunck has a different size we have to prepare buffers with
  // sizes and displacements of the chunck we have to send
  std::vector<int> sizes(size), displs(size + 1);
  for (int i = 0; i < size; ++i) {
    sizes[i] = n / size + (n % size > i);
    displs[i + 1] = displs[i] + sizes[i];
  }

  // scatter the chuncks
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

  // test for different sizes of vec
  for (const size_t n : {5, 6, 7, 100000, 200000}) {
    const std::vector<double> x = get_test(n);
    double xx;
    // call the function we have implemented and evaluate in how much time it runs
    const auto dt = timeit([&]() { xx = inner_product_v1(x, x); });
    // output results if this is proc 0
    if (rank == 0) {
      std::cout << "Elapsed: " << dt << " | Error: "
                << xx - (n * (n + 1) * (2 * n + 1)) / 6.0 << std::endl;
    }
  }

  MPI_Finalize();
  return 0;
}