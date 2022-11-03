#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <random>
#include <chrono>
#include <string>
#include <numbers>

auto timeit(const std::function<void()>& f) {
  using namespace std::chrono;
  const auto t0 = high_resolution_clock::now();
  f();
  const auto t1 = high_resolution_clock::now();
  return duration_cast<milliseconds>(t1 - t0).count();
}

std::pair<double, double> montecarlo(const std::function<double(double)>& f,
  unsigned long N) {
  // initialize random engine
  std::mt19937 engine(0);
  std::uniform_real_distribution<double> dist(-1., 1.);

  // get samples
  double sum = 0., sum_sq = 0.;
  for (auto i = 0ul; i < N; ++i) {
    const auto fi = f(dist(engine));
    sum += fi;
    sum_sq += fi * fi;
  }

  // compute integral
  const auto domain_measure = 2.0;
  const double integral = domain_measure * sum / N;
  const double variance = (sum_sq - (sum * sum) / N) / (N - 1);
  const double integral_variance = domain_measure * domain_measure * variance / N;
  return { integral, integral_variance };
}

int main(int argc, char* argv[]) {
  // get number of samples to use from argv
  const size_t N = argc > 1 ? std::stoull(argv[1]) : 100000;
  // compute the integral and time execution time
  std::pair<double, double> I;
  const auto dt = timeit([&]() {
    I = montecarlo([](auto x) { return std::sqrt(1 - x * x); }, N);
    });
  // output results
  std::cout << "Elapsed: " << dt << " [ms]" << std::endl;
  std::cout << "Integral: " << I.first << std::endl;
  std::cout << "Error estimator: " << std::sqrt(I.second) << std::endl;
  std::cout << "Error: " << std::abs(I.first - std::numbers::pi / 2) << std::endl;
  return 0;
}