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
  // FILL HERE

  // we assume the number of rows is divisible by the number of procs
  // compute the size of rows of A that is owned by each proc, and the respective
  // number of elements (if we have nrows of A, how many elements of A do we own?)
  // FILL HERE

  // allocate memory for local data of A and b (each proc owns a copy of the whole b)
  // FILL HERE

  // broadcast the data of b and scatter the data of A
  // FILL HERE

  // initialize error and number of iterations
  // FILL HERE
  while (/*FILL HERE: while the error is greater than the tolerance and we do not exceed the max number of iterations*/ ) {
    // compute local matrix-vector product, use Eigen `*`
    // FILL HERE

    // allocate memory for the whole b at this new iteration, we need both the new value and old value of b
    // FILL HERE

    // allgather b in the just created buffer
    // FILL HERE

    // normalize the new value of b, you can use the Eigen::VectorXd method norm() do 
    // FILL HERE

    // compute the error as the norm of the difference between the the old and the new b
    // FILL HERE

    // swap b and b_new since we do not need the old b anymore 
    // FILL HERE

    // update iteration count
    // FILL HERE
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