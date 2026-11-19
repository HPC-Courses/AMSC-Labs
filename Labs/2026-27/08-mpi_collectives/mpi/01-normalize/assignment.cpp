/*
Normalise a vector, x / |x|, with several processes. Only rank 0 has x: it
sends a part of x to each process, each process works on its own part, and
rank 0 collects the parts.

compile with
  mpic++ -Wall -std=c++20 assignment.cpp -o assignment
and run it with 4 processes with
  mpirun -n 4 ./assignment
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

  // TODO 1: all the processes need n. Rank 0 sends it to all the others,
  //         with MPI_Bcast.

  // TODO 2: each process receives its block of x in local_x, with MPI_Scatter.
  //         The arguments: what rank 0 sends, how many numbers to EACH
  //         process, their type, where each process receives, how many
  //         numbers, their type, the root, the communicator.
  const int local_n = n / size;
  std::vector<double> local_x(local_n);

  std::cout << "rank " << rank << ": " << to_string("local_x", local_x) << std::endl;

  // The sum of the squares of the local part
  double local_sum = 0;
  for (double xi : local_x)
    local_sum += xi * xi;

  // TODO 3: the norm of x, on ALL the processes: sum the local_sum of all
  //         the processes with MPI_Allreduce (as MPI_Reduce, without the
  //         root), then take the square root. Each process prints the norm,
  //         and divides its local_x by it.
  double norm = 0;
  (void)norm; // only to avoid the warning "unused variable": remove it

  // TODO 4: rank 0 collects the local_x of all the processes into x, with
  //         MPI_Gather, and prints x.

  MPI_Finalize();
  return 0;
}
