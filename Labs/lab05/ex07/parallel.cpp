#include <chrono>
#include <iostream>
#include <thread>

#include <mpi.h>
#include <Eigen/Eigen>

#include "../utils.hpp"


void power_method(Eigen::MatrixXd &A, Eigen::VectorXd &b) {
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  unsigned long N = b.size();
  MPI_Bcast(&N, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
  // we assume the number of rows is divisible by the number of procs
  const unsigned long local_rows = N / size;
  const unsigned long local_entries = local_rows * N;
  if (rank) {
    b.resize(N);
    A.resize(local_rows, N);
  }
  MPI_Bcast(b.data(), N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Scatter(A.data(), local_entries, MPI_DOUBLE, A.data(), local_entries,
              MPI_DOUBLE, 0, MPI_COMM_WORLD);

  double err = 1;
  size_t iter = 0;
  while (err > 1e-8 && iter < 100000) {
    Eigen::VectorXd b_local = A * b;
    Eigen::VectorXd b_new(b.size());
    MPI_Allgather(
      b_local.data(),
      local_rows,
      MPI_DOUBLE,
      b_new.data(),
      local_rows,
      MPI_DOUBLE,
      MPI_COMM_WORLD
    );
    // norm could be computed in parallel but we settle for serial
    b_new /= b_new.norm();
    err = (b_new - b).norm();
    b = b_new;
    iter++;
  }
  if(rank == 0) {
    std::cout << "Iterations: " << iter << std::endl;
    std::cout << "Error: " << err << std::endl;
  }
}

int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const unsigned long N = 100;
  Eigen::MatrixXd A;
  Eigen::VectorXd b;
  if (rank == 0) {  // simulate only the main proc has the data
    b = Eigen::VectorXd::Ones(N);
    A = Eigen::MatrixXd::Zero(N, N);
    A.diagonal(-1).setConstant(-1.0);
    A.diagonal(0).setConstant(2.0);
    A.diagonal(1).setConstant(-1.0);
  }
  // ===============================================================
  const auto dt = timeit([&](){power_method(A, b);});
  if(rank == 0)
    std::cout << "Elapsed: " << dt << std::endl;

  MPI_Finalize();
  return 0;
}