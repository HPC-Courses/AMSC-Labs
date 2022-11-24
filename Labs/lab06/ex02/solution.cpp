#include <iostream>
#include <chrono>

#include <Eigen/Eigen>
#include <omp.h>

using namespace std::chrono;
using Matrix = Eigen::Matrix<double, -1, -1, Eigen::RowMajor>;

int main(int argc, char** argv) {
  // parse inputs 
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << "  matrix size\n";
    std::exit(EXIT_FAILURE);
  }
  const auto n = atoi(argv[1]);

  // initialize matrices
  const Matrix A = Eigen::MatrixXd::Random(n, n);
  const Matrix B = Eigen::MatrixXd::Random(n, n);
  Matrix C = Eigen::MatrixXd::Zero(n, n);
  
  // make naive matrix multiplication
  const auto t0 = high_resolution_clock::now();
  #pragma omp parallel for
  for (int i = 0; i < n; i++)
    for (int k = 0; k < n; k++)
      for (int j = 0; j < n; j++)
        C.coeffRef(i, j) += A.coeffRef(i, k) * B.coeffRef(k, j);
  const auto t1 = high_resolution_clock::now();
  const auto dt = duration_cast<milliseconds>(t1 - t0).count();
  std::cout << "Elapsed mm naive: " << dt << " [ms]\n";

  // check result is correct
  const auto t2 = high_resolution_clock::now();
  Matrix Ctrue = A * B;
  const auto t3 = high_resolution_clock::now();
  Eigen::MatrixXd err = (Ctrue - C).cwiseAbs();
  std::cout << "Elapsed mm Eigen: " << duration_cast<milliseconds>(t3 - t2).count() << " [ms]\n";
  std::cout << "Error: " << err.maxCoeff() << std::endl;

  return 0;
}