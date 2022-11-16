#include <chrono>
#include <iostream>
#include <thread>

#include <mpi.h>

#include <Eigen/Eigen>

#include "../utils.hpp"

using Matrix = Eigen::Matrix<double, -1, -1, Eigen::RowMajor>;

void power_method(const Matrix &A, Eigen::VectorXd &b, size_t max_iter, double toll) {
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // broadcast the size of b
  unsigned long N = b.size();
  MPI_Bcast(&N, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
  // we assume the number of rows is divisible by the number of procs
  const unsigned long local_rows = N / size;
  const unsigned long local_entries = local_rows * N;
  // allocate buffer for local data
  if (rank) {
    b.resize(N);
  }
  Matrix local_A(local_rows, N);

  // broadcast the data of b and scatter the data of A
  MPI_Bcast(b.data(), N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Scatter(A.data(), local_entries, MPI_DOUBLE, local_A.data(), local_entries,
              MPI_DOUBLE, 0, MPI_COMM_WORLD);

  double err = 1;
  size_t iter = 0;
  while (err > toll && iter < max_iter) {
    // compute local matrix-vector product
    Eigen::VectorXd b_local = local_A * b;
    // allocate memory for the whole b
    Eigen::VectorXd b_new(b.size());
    // allgather b
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
    // this is suboptimal, but in this case it is not that bad of a choice 
    // since we already have the whole vector b in local memory and 
    // in this way we avoid extra communications
    b_new /= b_new.norm();
    err = (b_new - b).norm();
    // swap b and b_new
    std::swap(b, b_new);
    iter++;
  }
  if(rank == 0) {
    std::cout << "Iterations: " << iter << "\n";
    std::cout << "Error: " << err << "\n";
  }
}

int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const unsigned long N = 400;
  Eigen::Matrix<double, -1, -1, Eigen::RowMajor> A;
  Eigen::VectorXd b;
  if (rank == 0) {  // simulate only the main proc has the data
    // Ideally choose a random vector to decrease the chance that our vector
    // is orthogonal to the eigenvector
    b = Eigen::VectorXd::Random(N);
    A = Eigen::MatrixXd::Zero(N, N);
    A.diagonal(-1).setConstant(-1.0);
    A.diagonal(0).setConstant(2.0);
    A.diagonal(1).setConstant(-1.0);
  }
  // time our function
  const auto dt = timeit([&](){power_method(A, b, 1000000, 1e-8);});
  // output result if we are proc 0
  if(rank == 0) {
    std::cout << "Elapsed: " << dt << std::endl;
    const auto eigenvalue = (b.transpose()*A*b) / (b.transpose()*b);
    std::cout << "Eigenvalue: " << eigenvalue << std::endl;

    // test out solution against Eigen
    Eigen::EigenSolver<Matrix> es(A);
    Eigen::Index maxRow;
    const auto test_eigenvalue = es.eigenvalues().real().maxCoeff(&maxRow);
    std::cout << "The eigenvalue error against Eigen is: " << (eigenvalue(0, 0) - test_eigenvalue) << std::endl;
    std::cout << "The eigenvector error against Eigen is: " << (es.eigenvectors().real().col(maxRow) - b).norm() << std::endl;
  }

  MPI_Finalize();
  return 0;
}