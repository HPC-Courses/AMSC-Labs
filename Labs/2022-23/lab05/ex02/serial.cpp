#include <Eigen/Eigen>

#include "../utils.hpp"

void power_method(Eigen::MatrixXd &A, Eigen::VectorXd &b, size_t max_iter, double toll) {
  double err = 1.0 + toll;
  size_t iter = 0;
  while(err > toll && iter < max_iter) {
    Eigen::VectorXd b_new = A * b;
    b_new /= b_new.norm();
    err = (b_new - b).norm();
    // instead of doing `b = b_new` which means we are doing a useless copy
    // we swap the two vectors since we do not need the old value of `b` anymore
    std::swap(b, b_new);
    iter++;
  }
  std::cout << "Iterations: " << iter << "\n"; 
  std::cout << "Error: " << err << "\n";
}

int main() {
  const unsigned long N = 4;
  Eigen::MatrixXd A = Eigen::MatrixXd::Zero(N, N);
  // Ideally choose a random vector to decrease the chance that our vector
  // is orthogonal to the eigenvector
  Eigen::VectorXd b = Eigen::VectorXd::Random(N);
  A.diagonal(-1).setConstant(-1.0);
  A.diagonal(0).setConstant(2.0);
  A.diagonal(1).setConstant(-1.0);

  const auto dt = timeit([&](){power_method(A, b, 100000, 1e-8);});
  const auto eigenvalue = (b.transpose()*A*b) / (b.transpose()*b);
  std::cout << "Elapsed: " << dt << std::endl;
  std::cout << "Eigenvalue: " << eigenvalue << std::endl;
  std::cout << "Eigenvector: \n" << b << std::endl;

  // test out solution against Eigen
  Eigen::EigenSolver<Eigen::MatrixXd> es(A);
  Eigen::Index maxRow;
  const auto test_eigenvalue = es.eigenvalues().real().maxCoeff(&maxRow);
  const auto test_eigenvector = es.eigenvectors().real().col(maxRow);
  std::cout << "The eigenvalue error against Eigen is: " << (eigenvalue(0, 0) - test_eigenvalue) << std::endl;
  std::cout << "The eigenvector error against Eigen is: " << (b - test_eigenvector).norm() << std::endl;

  return 0;
}