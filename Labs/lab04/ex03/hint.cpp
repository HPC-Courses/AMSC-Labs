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
  // FILL HERE

  // broadcast the numer of samples to every proc
  if(/*FILL HERE: rank is not 0*/) {
    // FILL HERE: recieve N
  } else {
    // FILL HERE: send N to each proc (that is not 0)
  }

  // compute how many samples this proc handles
  const auto subsample = /*FILL HERE*/;

  // initialize random engine
  const auto seed = /*FILL HERE: find a way to produce a different seed for each proc*/;
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
  if(/*FILL HERE: rank is not 0*/) {
    // FILL HERE: send sum and sum_sq to proc 0
  } else {
    // FILL HERE: recieve sum and sum_sq from each proc and sum them together
  }

  // FILL HERE: if rank is 0 make the computations, otherwise return {0, 0}
}

int main(int argc, char* argv[]) {
  // init MPI, get rank and size
  // FILL HERE

  // get number of samples to use from argv
  const size_t N = argc > 1 ? std::stoull(argv[1]) : 100000;
  // compute the integral and time execution time
  std::pair<double, double> I;
  const auto dt = timeit([&]() {
    I = montecarlo([](auto x) { return std::sqrt(1 - x * x); }, N);
    });
  // output results
  if (/*FILL HERE*/) {
    std::cout << "Elapsed: " << dt << std::endl;
    std::cout << "Integral: " << I.first << std::endl;
    std::cout << "Error estimator: " << std::sqrt(I.second) << std::endl;
    std::cout << "Error: " << std::abs(I.first - std::numbers::pi / 2) << std::endl;
  }
  // Finalize MPI
  // FILL HERE
  return 0;
}