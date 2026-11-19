/*
The power method: the largest eigenvalue of a matrix A, and its eigenvector.
Start from a vector x with |x| = 1, and repeat

  y = A x,   lambda = |y|,   x = y / lambda

lambda tends to the largest eigenvalue, and x to its eigenvector. Solution.

Rank 0 has A. Each process receives a block of rows of A, and keeps the same
block of x. To compute its rows of A x, a process needs ALL of x.

compile with
  mpic++ -Wall -std=c++20 -O3 solution.cpp -o solution
and run it with
  mpirun -n 4 ./solution
It also works with -n 3: the blocks are 667, 667 and 666 rows.
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
  // 1. All the processes need n, to compute rows and first_row
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

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

  // 2. MPI_Scatterv counts numbers, not rows: a row is n numbers. Process r
  //    receives rows[r] * n numbers of A, from the position first_row[r] * n.
  std::vector<int> entries(size);
  std::vector<int> first_entry(size);
  for (int r = 0; r < size; ++r) {
    entries[r] = rows[r] * n;
    first_entry[r] = first_row[r] * n;
  }
  std::vector<double> local_A(local_rows * n);
  MPI_Scatterv(A.data(), entries.data(), first_entry.data(), MPI_DOUBLE, local_A.data(),
               local_rows * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // Each process has its block of x: the entries first_row[rank], ... We
  // start from x = (1, 1, ..., 1) / sqrt(n), so that |x| = 1.
  std::vector<double> local_x(local_rows, 1.0 / std::sqrt(n));
  std::vector<double> x(n); // all of x, on every process
  std::vector<double> local_y(local_rows);
  double lambda = 0;

  for (int k = 1; k <= 10; ++k) {
    // 3. Each process needs all of x: each one sends its block to all the
    //    others. As MPI_Gatherv, without the root: all the processes receive.
    MPI_Allgatherv(local_x.data(), local_rows, MPI_DOUBLE, x.data(), rows.data(), first_row.data(),
                   MPI_DOUBLE, MPI_COMM_WORLD);

    // The rows of y = A x of this process
    for (int i = 0; i < local_rows; ++i) {
      double yi = 0;
      for (int j = 0; j < n; ++j)
        yi += local_A[i * n + j] * x[j];
      local_y[i] = yi;
    }

    // 4. |y|, on all the processes, as the norm in mpi/01-normalize
    double local_sum = 0;
    for (double yi : local_y)
      local_sum += yi * yi;
    double sum = 0;
    MPI_Allreduce(&local_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    lambda = std::sqrt(sum);
    for (int i = 0; i < local_rows; ++i)
      local_x[i] = local_y[i] / lambda;

    if (rank == 0)
      std::cout << "iteration " << std::setw(2) << k << ": lambda = " << std::setprecision(15)
                << lambda << std::endl;
  }

  // 5. Rank 0 collects the blocks of x
  MPI_Gatherv(local_x.data(), local_rows, MPI_DOUBLE, x.data(), rows.data(), first_row.data(),
              MPI_DOUBLE, 0, MPI_COMM_WORLD);

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
