#include <Eigen/Eigen>

#include "../utils.hpp"

void power_method(Eigen::MatrixXd &A, Eigen::VectorXd &b) {
  double err = 1;
  size_t iter = 0;
  while(err > 1e-8 && iter < 100000) {
    Eigen::VectorXd b_new = A * b;
    b_new /= b_new.norm();
    err = (b_new - b).norm();
    b = b_new;
    iter++;
  }
  std::cout << "Iterations: " << iter << std::endl; 
  std::cout << "Error: " <<  err << std::endl; 
}

int main() {
  const unsigned long N = 100;
  Eigen::MatrixXd A = Eigen::MatrixXd::Zero(N, N);
  Eigen::VectorXd b = Eigen::VectorXd::Ones(N);
  A.diagonal(-1).setConstant(-1.0);
  A.diagonal(0).setConstant(2.0);
  A.diagonal(1).setConstant(-1.0);

  const auto dt = timeit([&](){power_method(A, b);});
  std::cout << "Elapsed: " << dt << std::endl;

  return 0;
}