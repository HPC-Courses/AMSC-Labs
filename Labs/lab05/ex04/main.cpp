#include <array>
#include <cmath>
#include <numeric>

#include <mpi.h>

#include "../utils.hpp"

double inner_product(const std::vector<double>& a,
                     const std::vector<double>& b) {
  if (a.size() != b.size()) {
    std::cerr << "Incompatible vector size" << std::endl;
    std::exit(-1);
  }

  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  unsigned long local_size = a.size() / size;
  MPI_Bcast(&local_size, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
  std::vector<double> local_a(local_size), local_b(local_size);

  MPI_Scatter(a.data(), local_size, MPI_DOUBLE, local_a.data(), local_size,
              MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Scatter(b.data(), local_size, MPI_DOUBLE, local_b.data(), local_size,
              MPI_DOUBLE, 0, MPI_COMM_WORLD);

  const double partial_reminder =
      rank ? 0.0
           : std::inner_product(a.cend() - a.size() % size, a.cend(),
                                b.cend() - b.size() % size, 0.0);
  const double partial =
      partial_reminder + std::inner_product(local_a.cbegin(), local_a.cend(),
                                            local_b.cbegin(), 0.0);
  double sum;
  MPI_Reduce(&partial, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  return sum;
}

double inner_product_v(const std::vector<double>& a,
                       const std::vector<double>& b) {
  if (a.size() != b.size()) {
    std::cerr << "Incompatible vector size" << std::endl;
    std::exit(-1);
  }

  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int n = a.size();
  MPI_Bcast(&n, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
  const auto local_size = n / size + (n % size > rank);
  std::vector<double> local_a(local_size), local_b(local_size);

  std::vector<int> sizes(size), displs(size + 1);
  for (int i = 0; i < size; ++i) {
    sizes[i] = n / size + (n % size > i);
    displs[i + 1] = displs[i] + sizes[i];
  }

  MPI_Scatterv(a.data(), sizes.data(), displs.data(), MPI_DOUBLE,
               local_a.data(), local_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Scatterv(b.data(), sizes.data(), displs.data(), MPI_DOUBLE,
               local_b.data(), local_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  const double partial = std::inner_product(local_a.cbegin(), local_a.cend(),
                                            local_b.cbegin(), 0.0);
  double sum;
  MPI_Reduce(&partial, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  return sum;
}

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

  for (const auto N :
       {5, 6, 7, 100000, 200000, 400000, 800000, 1600000, 3200000, 6400000}) {
    const std::vector<double> x = get_test(N);
    double xx;
    for (const auto& impl : {inner_product_v, inner_product}) {
      const auto dt = timeit([&]() { xx = impl(x, x); });
      if (rank == 0) {
        const auto n = double(N);
        std::cout << "Elapsed: " << dt << " " << xx << " "
                  << (n * (n + 1) * (2 * n + 1)) / 6 << std::endl;
      }
    }
  }

  MPI_Finalize();
  return 0;
}