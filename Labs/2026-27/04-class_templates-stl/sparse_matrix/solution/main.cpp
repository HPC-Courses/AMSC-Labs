#include "coo_matrix.hpp"
#include "csr_matrix.hpp" // homework
#include "map_matrix.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <unordered_map>

// Fills A with the matrix of the second derivative: -2 on the diagonal, 1 above
// and below. A function template: Matrix is any type with nrows() and A(i, j) = ...
// In a real code, e.g. a finite element assembly, the entries are not inserted
// row by row: with shuffle = true the rows are filled in a random order.
template <typename Matrix>
void fill_tridiagonal(Matrix &A, bool shuffle = false) {
  const std::size_t n = A.nrows();
  std::vector<std::size_t> rows(n);
  std::iota(rows.begin(), rows.end(), 0); // 0, 1, ..., n-1
  if (shuffle)
    std::shuffle(rows.begin(), rows.end(), std::mt19937{42});

  for (const std::size_t i : rows) {
    if (i > 0)
      A(i, i - 1) = 1.;
    A(i, i) = -2.;
    if (i + 1 < n)
      A(i, i + 1) = 1.;
  }
}

// Checks the product with x = (0, 1, ..., n-1): the result must be (1, 0, ..., 0, -n).
// It takes a reference to the base class: it works for every format
template <typename T>
bool check_vmult(const SparseMatrix<T> &A) {
  const std::size_t n = A.nrows();
  std::vector<T> x(n);
  std::iota(x.begin(), x.end(), T{0});

  std::vector<T> expected(n, T{0});
  expected.front() = 1;
  expected.back() = -static_cast<T>(n);
  // the entries are small integers, exactly representable: == is safe here
  return A.vmult(x) == expected;
}

std::string passed(bool ok) { return ok ? "PASSED" : "FAILED"; }

using Clock = std::chrono::steady_clock;

// Milliseconds elapsed since start
double ms_since(Clock::time_point start) {
  return std::chrono::duration<double, std::milli>(Clock::now() - start).count();
}

// Average time of one product y = A x, over 10 products
template <typename T>
double time_vmult(const SparseMatrix<T> &A) {
  const std::vector<T> x(A.ncols(), T{1});
  const auto start = Clock::now();
  for (int k = 0; k < 10; ++k)
    A.vmult(x);
  return ms_since(start) / 10;
}

// Map is a template template parameter, as in MapMatrix
template <template <typename...> class Map>
void benchmark(const std::string &name, std::size_t n) {
  auto start = Clock::now();
  MapMatrix<double, Map> A(n, n);
  fill_tridiagonal(A, true);
  const double t_fill = ms_since(start);
  std::cout << name << ": fill " << t_fill << " ms, vmult " << time_vmult(A) << " ms" << std::endl;

  start = Clock::now();
  const CooMatrix C{A};
  const double t_coo = ms_since(start);
  std::cout << "  COO: compress " << t_coo << " ms, vmult " << time_vmult(C) << " ms" << std::endl;

  // Homework
  start = Clock::now();
  const CsrMatrix R{A};
  const double t_csr = ms_since(start);
  std::cout << "  CSR: compress " << t_csr << " ms, vmult " << time_vmult(R) << " ms" << std::endl;
}

int main(int argc, char **argv) {
  std::cout << "Step 2: MapMatrix" << std::endl;
  MapMatrix<double> A(5, 5);
  fill_tridiagonal(A);
  A.print();
  std::cout << "vmult: " << passed(check_vmult(A)) << std::endl;

  // cA is a const reference to A: only the const operator() can be called on it
  const MapMatrix<double> &cA = A;
  std::cout << "cA(0, 4) = " << cA(0, 4) << ", nnz = " << A.nnz() << std::endl;
  // A is not const: which operator() is called here?
  std::cout << "A(4, 0) = " << A(4, 0) << ", nnz = " << A.nnz() << std::endl;
  try {
    A(5, 0) = 1.; // outside the matrix
  } catch (const std::out_of_range &e) {
    std::cout << "A(5, 0) = 1.: " << e.what() << std::endl;
  }

  std::cout << std::endl << "Step 3: unordered_map" << std::endl;
  MapMatrix<double, std::unordered_map> B(5, 5);
  fill_tridiagonal(B);
  B.print();
  std::cout << "vmult: " << passed(check_vmult(B)) << std::endl;

  std::cout << std::endl << "Step 4: COO" << std::endl;
  const CooMatrix C{B}; // CTAD: CooMatrix<double>
  C.print();
  std::cout << "vmult: " << passed(check_vmult(C)) << std::endl;
  std::cout << "C(1, 2) = " << C(1, 2) << ", C(0, 4) = " << C(0, 4) << std::endl;

  // Homework: CSR
  std::cout << std::endl << "Homework: CSR" << std::endl;
  const CsrMatrix R{B};
  std::cout << "vmult: " << passed(check_vmult(R)) << std::endl;
  std::cout << "R(1, 2) = " << R(1, 2) << ", R(0, 4) = " << R(0, 4) << std::endl;

  // ./main benchmark (or make benchmark)
  if (argc > 1 && std::string{argv[1]} == "benchmark") {
    const std::size_t n = 500'000;
    std::cout << std::endl << "Benchmark, n = " << n << std::endl;
    benchmark<std::map>("map", n);
    benchmark<std::unordered_map>("unordered_map", n);
  }
  return 0;
}
