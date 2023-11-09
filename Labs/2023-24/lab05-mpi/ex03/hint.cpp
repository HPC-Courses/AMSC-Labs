#include <mpi.h>

#include <array>
#include <cmath>
#include <numeric>

#include "../utils.hpp"

// In this version, if a and b have size `n` that is not exactly divisible
// by the number of procs `size`, we split the vectors in chunks of size
// `n / size` and the remainder chunk of size `n % size` is handled by
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
  // FILL HERE

  // allocate buffers
  // FILL HERE

  // scatter data
  // FILL HERE

  // compute the inner product of the reminder chuck `n % size` at the end of
  // `a` and `b` with `std::inner_product`
  double inner_prod_reminder /* = FILL HERE*/;

  // compute the inner product of the local chunk with `std::inner_product`
  // FILL HERE

  // the inner product is just the sum of the inner products of the local
  // chunks, use MPI_Reduce
  double sum;
  // FILL HERE
  return sum;
}

// In this version, if a and b have size `n` that is not exactly divisible
// by the number of procs `size`, we split the vectors in chunks of size
// `n / size` plus 1 if the remainder `n % size` is greater than `rank`.
// In this way we have `size` chunks : * `n % size` chunks of size `n / size +
// 1` and
//                                      * `size - n % size` chunks of size `n /
//                                      size`
// Since each chunk has a different size we have to employ `MPI_Scatterv` and
// specify where each chunk begins and which is its size.
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
  // FILL HERE

  // the local size is `n / size` plus 1 if the reminder `n % size` is greater
  // than `rank` in this way we split the load in the most equilibrate way FILL
  // HERE

  // since each chunk has a different size we have to prepare buffers with
  // sizes and displacements of the chunk we have to send
  // FILL HERE

  // scatter the chunks with MPI_Scatterv
  // FILL HERE

  // compute local inner product with `std::inner_product`
  // FILL HERE

  // sum together all local inner products, use MPI_Reduce
  double sum;
  // FILL HERE
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
    // call the function we have implemented and evaluate in how much time it
    // runs
    const auto dt = timeit([&]() { xx = inner_product_v1(x, x); });
    // output results if this is proc 0
    if (rank == 0) {
      std::cout << "Elapsed: " << dt
                << " | Error: " << xx - (n * (n + 1) * (2 * n + 1)) / 6.0
                << std::endl;
    }
  }

  MPI_Finalize();
  return 0;
}