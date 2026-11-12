#include "csr_matrix.hpp"
#include "map_matrix.hpp"

#include <chrono>
#include <iostream>
#include <numeric>
#include <string>
#include <thread>
#include <vector>

// Fills A with the matrix of the second derivative: -2 on the diagonal, 1 above
// and below (Lab 4)
void fill_tridiagonal(MapMatrix<double> &A) {
  const std::size_t n = A.nrows();
  for (std::size_t i = 0; i < n; ++i) {
    if (i > 0)
      A(i, i - 1) = 1.;
    A(i, i) = -2.;
    if (i + 1 < n)
      A(i, i + 1) = 1.;
  }
}

std::string passed(bool ok) { return ok ? "PASSED" : "FAILED"; }

using Clock = std::chrono::steady_clock;

// Milliseconds elapsed since start
double ms_since(Clock::time_point start) {
  return std::chrono::duration<double, std::milli>(Clock::now() - start).count();
}

int main(int argc, char **argv) {
  // The test of Lab 4: with x = (0, 1, ..., n-1), A x = (1, 0, ..., 0, -n)
  const std::size_t n = 5;
  MapMatrix<double> A(n, n);
  fill_tridiagonal(A);
  const CsrMatrix<double> R{A};
  R.print();

  std::vector<double> x(n);
  std::iota(x.begin(), x.end(), 0.);
  std::vector<double> expected(n, 0.);
  expected.front() = 1.;
  expected.back() = -static_cast<double>(n);
  std::cout << "vmult:          " << passed(R.vmult(x) == expected) << std::endl;
  std::cout << "vmult_parallel: " << passed(R.vmult_parallel(x) == expected) << std::endl;

  // ./main benchmark (or make benchmark)
  if (argc > 1 && std::string{argv[1]} == "benchmark") {
    const std::size_t big_n = 2'000'000;
    std::cout << std::endl << "Benchmark, n = " << big_n << ", threads: " << std::thread::hardware_concurrency()
              << std::endl;
    MapMatrix<double> big_A(big_n, big_n);
    fill_tridiagonal(big_A);
    const CsrMatrix<double> big_R{big_A};
    const std::vector<double> ones(big_n, 1.);

    // The first parallel call also creates the threads: we do not time it
    std::vector<double> y_parallel = big_R.vmult_parallel(ones);

    auto start = Clock::now();
    std::vector<double> y;
    for (int k = 0; k < 10; ++k)
      y = big_R.vmult(ones);
    std::cout << "vmult:          " << ms_since(start) / 10 << " ms" << std::endl;

    start = Clock::now();
    for (int k = 0; k < 10; ++k)
      y_parallel = big_R.vmult_parallel(ones);
    std::cout << "vmult_parallel: " << ms_since(start) / 10 << " ms" << std::endl;

    std::cout << "same result:    " << passed(y == y_parallel) << std::endl;
  }
  return 0;
}
