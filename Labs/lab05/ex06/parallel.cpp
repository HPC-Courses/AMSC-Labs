#include <Eigen/Eigen>
#include <iostream>
#include <chrono>
#include <functional>

#include <mpi.h>

auto timeit(const std::function<void()>& f) {
    using namespace std::chrono;
    const auto t0 = high_resolution_clock::now();
    f();
    const auto t1 = high_resolution_clock::now();
    return duration_cast<milliseconds>(t1 - t0).count();
}


int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::cout << "Successfully included Eigen." << std::endl;

  const unsigned long N = 400;
  const Eigen::MatrixXd A = Eigen::MatrixXd::Random(N, N);
  const Eigen::MatrixXd B = Eigen::MatrixXd::Random(N, N);

  Eigen::MatrixXd AB;
  const auto dt = timeit([&](){AB = A * B;});
  std::cout << dt << std::endl;
  
  Eigen::MatrixXd C(N, N);
  const auto dt2 = timeit([&](){
    C.block(0, 0, N/2, N/2) = A.middleRows(0, N/2) * B.middleCols(0, N/2);
    C.block(0, N/2, N/2, N/2) = A.middleRows(0, N/2) * B.middleCols(N/2, N/2);
    C.block(N/2, 0, N/2, N/2) = A.middleRows(N/2, N/2) * B.middleCols(0, N/2);
    C.block(N/2, N/2, N/2, N/2) = A.middleRows(N/2, N/2) * B.middleCols(N/2, N/2);
  });
  std::cout << dt2 << std::endl;


  std::cout << (C == AB) << std::endl;


  MPI_Finalize();
  return 0;
}