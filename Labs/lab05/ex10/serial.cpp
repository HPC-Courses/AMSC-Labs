#include <cmath>

#include "../utils.hpp"

// trapezoidal rule for functions in the interval (-1, 1) over N points
double trapezoidal_rule(const std::function<double(double)> &f, size_t N) {
  const double h = 2.0 / N;
  double I = f(-1) + f(1);
  for(size_t i = 1; i < N; ++i) {
    const double xi = -1.0 + h * i;
    I += 2.0*f(xi);
  }
  return I * h / 2.0;
}

int main() {
  double I;
  const auto dt = timeit([&](){
    I = trapezoidal_rule([](auto x){return std::sqrt(1 - x*x);}, 4);
  });
  std::cout << "Elapsed: " << dt << std::endl;
  std::cout << "Result: " << I << std::endl;
  std::cout << "Error: " << std::abs(I - M_PI/2.0) << std::endl;
  return 0;
}