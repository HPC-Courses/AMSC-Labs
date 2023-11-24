#include <mpi.h>
#include <omp.h>

#include <chrono>
#include <cmath>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <tuple>
#include <vector>

using namespace std::chrono;

inline double simpson_multithreaded(std::function<double(double)> const& f,
                                    double a, double b, unsigned int n,
                                    unsigned int num_threads) {
  double integral = 0.0;
  double h = (b - a) / n;
  // FILL HERE: add OpenMP directive to parallelize the loop, specify
  // - the number of threads `num_threads` to use
  // - the shared variables
  // - the reduction operation
  for (auto i = 0u; i < n; ++i) {
    integral +=
        (f(a + i * h) + 4. * f(a + (i + 0.5) * h) + f(a + (i + 1.) * h));
  }
  return (h / 6.) * integral;
}

int main(int argc, char** argv) {
  // parse arguments
  // - the number of intervals in which [0,1] is split into when evaluating the
  // integral
  // - the number of OpenMP threads we want to use
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0]
              << " number-of-intervals number-of-threads\n";
    std::exit(EXIT_FAILURE);
  }
  const auto n = atoi(argv[1]);
  const auto nthreads = atoi(argv[2]);

  // initialize MPI
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // notice that differently from the other labs we assume that all
  // processors know  the total number of intervals and that we are
  // working in [0, 1] so the broadcast is not needed
  // --------------------------------------------------------------
  // number of intervals handled by this proc
  const auto n_local /*= FILL HERE*/;
  // size of the partition of the interval [0, 1], you can use equal sized
  // partitions
  const auto local_interval_size /*= FILL HERE*/;
  // start of the partition for this proc
  const auto a_local /*= FILL HERE*/;

  // compute the integral and measure elapsed time
  const auto t0 = high_resolution_clock::now();
  const double local_integral = simpson_multithreaded(
      [](auto x) { return std::sqrt(1 - x * x); }, a_local,
      a_local + local_interval_size, n_local, nthreads);
  double integral = 0.0;
  // use MPI_reduce to compute the value of the integral
  // as the sum on the integrals on the partitions of [0, 1]
  // FILL HERE
  const auto t1 = high_resolution_clock::now();
  const auto dt = duration_cast<milliseconds>(t1 - t0).count();

  if (rank == 0) {
    std::cout << std::setprecision(16);
    std::cout << "Integral value: " << integral << "\n";
    std::cout << "Error: " << std::abs(integral * 4.0 - M_PI) << "\n";
    std::cout << "Elapsed:" << dt << " [ms]\n";
  }
  MPI_Finalize();
  return 0;
}
