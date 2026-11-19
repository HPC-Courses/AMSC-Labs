/*
The power method: the largest eigenvalue of a matrix A, and its eigenvector.
Start from a vector x with |x| = 1, and repeat

  y = A x,   lambda = |y|,   x = y / lambda

lambda tends to the largest eigenvalue, and x to its eigenvector.

Rank 0 has A. Each process receives a block of rows of A, and keeps the same
block of x. To compute its rows of A x, a process needs ALL of x.

compile with
  mpic++ -Wall -std=c++20 -O3 assignment.cpp -o assignment
and run it with
  mpirun -n 4 ./assignment
*/

#include <mpi.h>

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numbers>
#include <vector>

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  int size;
  int rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Only rank 0 has A, as if it had read it from a file. A is n x n, stored by
  // rows: the entry (i, j) is A[i * n + j] = min(i, j) + 1.
  //   1 1 1 1 ...
  //   1 2 2 2 ...
  //   1 2 3 3 ...
  //   1 2 3 4 ...
  int n = 0;
  std::vector<double> A;
  if (rank == 0) {
    n = 2000;
    A.resize(n * n);
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j)
        A[i * n + j] = std::min(i, j) + 1;
  }
  // TODO 1: all the processes need n, to compute rows and first_row. Rank 0
  //         sends it to all the others, with MPI_Bcast.

  // The rows of each process, and the first one: as counts and displacements
  // in mpi/01-normalize
  std::vector<int> rows(size);
  std::vector<int> first_row(size, 0);
  for (int r = 0; r < size; ++r) {
    rows[r] = n / size + (r < n % size ? 1 : 0);
    if (r > 0)
      first_row[r] = first_row[r - 1] + rows[r - 1];
  }
  const int local_rows = rows[rank];

  // TODO 2: each process receives its rows of A in local_A, with
  //         MPI_Scatterv. MPI_Scatterv counts numbers, not rows: build two
  //         vectors entries and first_entry, as rows and first_row, but in
  //         numbers of A.
  std::vector<double> local_A(local_rows * n);

  // Each process has its block of x: the entries first_row[rank], ... We
  // start from x = (1, 1, ..., 1) / sqrt(n), so that |x| = 1.
  std::vector<double> local_x(local_rows, 1.0 / std::sqrt(n));
  std::vector<double> x(n); // all of x, on every process
  std::vector<double> local_y(local_rows);
  double lambda = 0;

  for (int k = 1; k <= 10; ++k) {
    // TODO 3: each process needs all of x, and has only its block local_x:
    //         with MPI_Allgatherv, each process sends its block to all the
    //         others, and receives all the blocks into x.

    // The rows of y = A x of this process
    for (int i = 0; i < local_rows; ++i) {
      double yi = 0;
      for (int j = 0; j < n; ++j)
        yi += local_A[i * n + j] * x[j];
      local_y[i] = yi;
    }

    // |y|, on all the processes
    double local_sum = 0;
    for (double yi : local_y)
      local_sum += yi * yi;
    // TODO 4: sum the local_sum of all the processes, on all the processes,
    //         as the norm in mpi/01-normalize
    double sum = local_sum;
    lambda = std::sqrt(sum);
    for (int i = 0; i < local_rows; ++i)
      local_x[i] = local_y[i] / lambda;

    if (rank == 0)
      std::cout << "iteration " << std::setw(2) << k << ": lambda = " << std::setprecision(15)
                << lambda << std::endl;
  }

  // TODO 5: rank 0 collects the blocks local_x into x, with MPI_Gatherv,
  //         to compare x with the exact eigenvector

  if (rank == 0) {
    // For this A the eigenvalues and the eigenvectors are known: the largest
    // eigenvalue is 1 / (4 sin(theta / 2)^2), with theta = pi / (2n + 1), and
    // its eigenvector has the entries sin((i + 1) theta), i = 0, ..., n-1.
    const double theta = std::numbers::pi / (2 * n + 1);
    const double s = std::sin(theta / 2);
    const double lambda_exact = 1 / (4 * s * s);
    std::vector<double> v(n);
    double norm_v = 0;
    for (int i = 0; i < n; ++i) {
      v[i] = std::sin((i + 1) * theta);
      norm_v += v[i] * v[i];
    }
    norm_v = std::sqrt(norm_v);
    double error_x = 0;
    for (int i = 0; i < n; ++i)
      error_x = std::max(error_x, std::abs(x[i] - v[i] / norm_v));
    std::cout << "exact:        lambda = " << lambda_exact << std::endl;
    std::cout << std::setprecision(2) << size << " processes, n = " << n
              << ": error on lambda = " << std::abs(lambda - lambda_exact) / lambda_exact
              << ", error on x = " << error_x << std::endl;
  }

  MPI_Finalize();
  return 0;
}
