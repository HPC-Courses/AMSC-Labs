#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <random>
#include <chrono>
#include <string>
#include <numbers>

#include <mpi.h>


auto timeit(const std::function<void()>& f) {
  using namespace std::chrono;
  const auto t0 = high_resolution_clock::now();
  f();
  const auto t1 = high_resolution_clock::now();
  return duration_cast<milliseconds>(t1 - t0).count();
}

std::pair<double, double> montecarlo(const std::function<double(double)>& f,
  unsigned long N) {
  // get rank and size
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // broadcast the numer of samples to every proc
  if(rank) {
    MPI_Recv(&N, 1, MPI_UNSIGNED_LONG, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  } else {
    for (int i = 1; i < size; ++i) {
      MPI_Send(&N, 1, MPI_UNSIGNED_LONG, i, 0, MPI_COMM_WORLD);
    }
  }
  // or better
  // MPI_Bcast(&N, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

  // compute how many samples this proc handles
  const auto subsample = N / size + (unsigned(rank) < (N % size));

  // initialize random engine
  const auto seed = rank * rank * size * size;
  std::mt19937 engine(seed);
  std::uniform_real_distribution<double> dist(-1., 1.);

  // get samples
  double sum = 0., sum_sq = 0.;
  for (auto i = 0ul; i < subsample; ++i) {
    const auto fi = f(dist(engine));
    sum += fi;
    sum_sq += fi * fi;
  }

  // send local mean and sum of squares to proc 0
  if(rank) {
    MPI_Send(&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    MPI_Send(&sum_sq, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
  } else {
    for (int i = 1; i < size; ++i) {
      double tmp;
      MPI_Recv(&tmp, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      sum += tmp;
      MPI_Recv(&tmp, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      sum_sq += tmp;
    }
  }
  // or better
  // double tmp;
  // MPI_Reduce(&sum, &tmp, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  // sum = tmp;
  // MPI_Reduce(&sum_sq, &tmp, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  // sum_sq = tmp;

  if (rank == 0) {
    const auto domain_measure = 2.0;
    const double integral = domain_measure * sum / N;
    const double variance = (sum_sq - (sum * sum) / N) / (N - 1);
    const double integral_variance = domain_measure * domain_measure * variance / N;
    return { integral, integral_variance };
  }
  else {
    return { 0, 0 };
  }
}

int main(int argc, char* argv[]) {
  // init MPI, get rank and size
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // get number of samples to use from argv
  const size_t N = argc > 1 ? std::stoull(argv[1]) : 100000;
  // compute the integral and time execution time
  std::pair<double, double> I;
  const auto dt = timeit([&]() {
    I = montecarlo([](auto x) { return std::sqrt(1 - x * x); }, N);
    });
  // output results
  if (rank == 0) {
    std::cout << "Elapsed: " << dt << " [ms]" << std::endl;
    std::cout << "Integral: " << I.first << std::endl;
    std::cout << "Error estimator: " << std::sqrt(I.second) << std::endl;
    std::cout << "Error: " << std::abs(I.first - std::numbers::pi / 2) << std::endl;
  }
  MPI_Finalize();
  return 0;
}