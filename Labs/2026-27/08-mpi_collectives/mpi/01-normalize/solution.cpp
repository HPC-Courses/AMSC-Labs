/*
Normalise a vector, x / |x|, with several processes. Solution.

compile with
  mpic++ -Wall -std=c++20 solution.cpp -o solution
and run it with 4 processes with
  mpirun -n 4 ./solution
It also works with -n 3: the blocks are 3, 3 and 2 numbers.
*/

#include <mpi.h>

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// "<name> = v0 v1 ...", in one string: each process prints it with one <<,
// so that the lines of different processes do not get mixed
std::string to_string(const std::string &name, const std::vector<double> &v) {
  std::ostringstream line;
  line.precision(3);
  line << name << " =";
  for (double vi : v)
    line << " " << vi;
  return line.str();
}

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  int size;
  int rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Only rank 0 has the vector, as if it had read it from a file. On the
  // other processes x is empty, and n is 0.
  std::vector<double> x;
  int n = 0;
  if (rank == 0) {
    x = {1, 2, 3, 4, 5, 6, 7, 8};
    n = x.size();
    std::cout << to_string("x", x) << std::endl;
  }

  // 1. Rank 0 sends n to all the processes. All of them call MPI_Bcast: on
  //    rank 0 n is what is sent, on the others it is where it is received.
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // 2. How many numbers each process gets, and where its block starts in x.
  //    With MPI_Scatter every process gets n / size numbers: with -n 3,
  //    2 + 2 + 2, and the last 2 numbers of x are sent to nobody (task 6).
  //    Here the first n % size processes get one number more: 3 + 3 + 2.
  //    All the processes know n, so each one can compute counts by itself.
  std::vector<int> counts(size);
  std::vector<int> displacements(size, 0);
  for (int r = 0; r < size; ++r) {
    counts[r] = n / size + (r < n % size ? 1 : 0);
    if (r > 0)
      displacements[r] = displacements[r - 1] + counts[r - 1];
  }
  const int local_n = counts[rank];
  std::vector<double> local_x(local_n);

  // With blocks of the same size:
  //   MPI_Scatter(x.data(), local_n, MPI_DOUBLE, local_x.data(), local_n,
  //               MPI_DOUBLE, 0, MPI_COMM_WORLD);
  // On the processes other than 0, x is empty: what to send is read only on
  // the root.
  MPI_Scatterv(x.data(), counts.data(), displacements.data(), MPI_DOUBLE, local_x.data(), local_n,
               MPI_DOUBLE, 0, MPI_COMM_WORLD);

  std::cout << "rank " << rank << ": " << to_string("local_x", local_x) << std::endl;

  // The sum of the squares of the local part
  double local_sum = 0;
  for (double xi : local_x)
    local_sum += xi * xi;

  // 3. All the processes need the norm, not only rank 0: MPI_Allreduce.
  //    With MPI_Reduce, the norm of ranks 1, 2, 3 would be sqrt(0) = 0, and
  //    their local_x would become inf.
  double sum = 0;
  MPI_Allreduce(&local_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  const double norm = std::sqrt(sum);
  std::cout << "rank " << rank << ": norm = " << norm << std::endl;
  for (double &xi : local_x)
    xi /= norm;

  // 4. Rank 0 collects the blocks into x, each one at its displacement.
  //    With blocks of the same size:
  //      MPI_Gather(local_x.data(), local_n, MPI_DOUBLE, x.data(), local_n,
  //                 MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Gatherv(local_x.data(), local_n, MPI_DOUBLE, x.data(), counts.data(), displacements.data(),
              MPI_DOUBLE, 0, MPI_COMM_WORLD);
  if (rank == 0)
    std::cout << to_string("x / |x|", x) << std::endl;

  MPI_Finalize();
  return 0;
}
