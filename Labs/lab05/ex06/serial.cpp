#include <iostream>

#include <Eigen/Eigen>
#include <mpi.h>

#include "../utils.hpp"

int main(int argc, char* argv[]) {
  const unsigned long N = 400;
  const Eigen::Matrix<double, -1, -1, Eigen::RowMajor> A = Eigen::MatrixXd::Random(N, N);
  const Eigen::Matrix<double, -1, -1, Eigen::ColMajor> B = Eigen::MatrixXd::Random(N, N);

  // standard matrix vector multiplication
  Eigen::MatrixXd AB(N, N);
  const auto dt = timeit([&](){AB = A * B;});
  std::cout << dt << std::endl;
  
  Eigen::MatrixXd AB_parallel(N, N);
  const auto dt2 = timeit([&](){
    // what we would like to do in parallel:
    // each proc multiplies a block of the matrix
    // namely if we suppose to have four procs
    // each one of them executes one of the four lines below
    // See this link for details about the operations used below
    // https://eigen.tuxfamily.org/dox/group__TutorialBlockOperations.html
    AB_parallel.block(0, 0, N/2, N/2) = A.middleRows(0, N/2) * B.middleCols(0, N/2);
    AB_parallel.block(0, N/2, N/2, N/2) = A.middleRows(0, N/2) * B.middleCols(N/2, N/2);
    AB_parallel.block(N/2, 0, N/2, N/2) = A.middleRows(N/2, N/2) * B.middleCols(0, N/2);
    AB_parallel.block(N/2, N/2, N/2, N/2) = A.middleRows(N/2, N/2) * B.middleCols(N/2, N/2);
  });
  std::cout << dt2 << std::endl;

  // check the result is correct
  std::cout << (AB_parallel == AB) << std::endl;
  return 0;
}