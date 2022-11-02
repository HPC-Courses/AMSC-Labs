#include <cmath>

#include <mpi.h>

#include "../utils.hpp"

// trapezoidal rule for functions in the interval (-1, 1) over N points
double trapezoidal_rule(const std::function<double(double)>& f, size_t N) {
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const double h = 2.0 / N;
  double globalI = f(-1) + f(1);
  double I = 0;
  for (size_t i = rank + size * (rank == 0); i < N; i += size) {
    const double xi = -1.0 + h * i;
    I += 2.0 * f(xi);
  }
  MPI_Reduce(&I, &globalI, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  return globalI * h / 2.0;
}

int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double I;
  const auto dt = timeit([&]() {
    I = trapezoidal_rule([](auto x) { return x*x; }, 100000000);
  });
  if (rank == 0) {
    std::cout << "Elapsed: " << dt << std::endl;
    std::cout << "Result: " << I << std::endl;
    std::cout << "Error: " << std::abs(I - 2.0 / 3.0) << std::endl;
  }

  MPI_Finalize();
  return 0;
}