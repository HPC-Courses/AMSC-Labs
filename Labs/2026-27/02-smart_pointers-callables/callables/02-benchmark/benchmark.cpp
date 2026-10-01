/*
How much does the choice of the callable cost?
The same midpoint quadrature rule, with the integrand passed as
  - a template parameter (lambda or function),
  - a function pointer,
  - a std::function.

compile and run with different optimisation levels:
  g++ -Wall -std=c++20 -O0 benchmark.cpp -o benchmark_O0 && ./benchmark_O0
  g++ -Wall -std=c++20 -O3 benchmark.cpp -o benchmark_O3 && ./benchmark_O3
*/

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <string>

// Integrands: cheap on purpose, so that the cost of the call is visible
double square(double x) { return x * x; }
double cube(double x) { return x * x * x; }

// [[gnu::noinline]] forbids the compiler to copy the body of the quadrature into
// main, where it would "see" which function is passed and remove the indirection.
// It mimics the common situation of an algorithm compiled in another .cpp file.

template <typename F>
[[gnu::noinline]] double midpoint_template(F const &f, double a, double b, unsigned n) {
  double const h = (b - a) / n;
  double s = 0.;
  for (unsigned i = 0; i < n; ++i)
    s += f(a + (i + 0.5) * h);
  return s * h;
}

[[gnu::noinline]] double midpoint_pointer(double (*f)(double), double a, double b, unsigned n) {
  double const h = (b - a) / n;
  double s = 0.;
  for (unsigned i = 0; i < n; ++i)
    s += f(a + (i + 0.5) * h);
  return s * h;
}

[[gnu::noinline]] double midpoint_function(std::function<double(double)> const &f, double a, double b,
                                           unsigned n) {
  double const h = (b - a) / n;
  double s = 0.;
  for (unsigned i = 0; i < n; ++i)
    s += f(a + (i + 0.5) * h);
  return s * h;
}

// Runs a callable a few times and prints the best elapsed time
template <typename Run>
void time_it(std::string const &name, Run const &run) {
  double best = 1e30;
  // writing to a volatile variable is an observable side effect: the computation
  // can be neither removed nor moved outside the timed region
  volatile double result = 0.;
  for (int rep = 0; rep < 5; ++rep) {
    auto const start = std::chrono::steady_clock::now();
    result = run();
    auto const stop = std::chrono::steady_clock::now();
    best = std::min(best, std::chrono::duration<double, std::milli>(stop - start).count());
  }
  std::cout << name << ": result " << result << ", " << best << " ms" << std::endl;
}

int main(int argc, char **) {
  unsigned const n = 50'000'000;
  double const a = 0.;
  // volatile: re-read at every repetition, otherwise the compiler may notice that
  // repeating the same pure computation is useless and skip it
  volatile double b = 1.;
  auto const lambda = [](double x) { return x * x; };
  // Chosen at run time (argc is 1 unless you pass arguments): the compiler cannot
  // know which function the pointer and the std::function will call
  double (*const fptr)(double) = (argc > 1) ? cube : square;
  std::function<double(double)> const fun = fptr;

  time_it("template + lambda     ", [&] { return midpoint_template(lambda, a, b, n); });
  time_it("template + function   ", [&] { return midpoint_template(square, a, b, n); });
  time_it("function pointer      ", [&] { return midpoint_pointer(fptr, a, b, n); });
  time_it("std::function         ", [&] { return midpoint_function(fun, a, b, n); });
  return 0;
}
