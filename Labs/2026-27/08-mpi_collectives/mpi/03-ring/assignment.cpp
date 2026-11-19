/*
A ring: each process sends a message to the next one, rank + 1, and receives
one from the previous one, rank - 1. The last process sends to rank 0.

  rank 0 --> rank 1 --> rank 2 --> rank 3
    ^                                |
    +--------------------------------+

compile with
  mpic++ -Wall -std=c++20 assignment.cpp -o assignment
and run it with 4 processes, and messages of m numbers, with
  mpirun -n 4 ./assignment 1
  mpirun -n 4 ./assignment 100000
*/

#include <mpi.h>

#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  int size;
  int rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // How many numbers in each message: the first argument of the program
  const int m = argc > 1 ? std::stoi(argv[1]) : 1;

  const int next = (rank + 1) % size;            // where we send
  const int previous = (rank - 1 + size) % size; // from where we receive

  std::vector<int> message(m, rank); // m copies of our rank
  std::vector<int> received(m, -1);

  // TODO 1: send message to next with MPI_Send, then receive into received
  //         from previous with MPI_Recv. Use the tag 0, and MPI_STATUS_IGNORE
  //         as the status of MPI_Recv.

  // TODO 2: (task 3) replace the two calls with one MPI_Sendrecv.

  (void)next; // only to avoid the warning "unused variable": remove it

  std::cout << "rank " + std::to_string(rank) + ": received " + std::to_string(received[0]) +
                   " from rank " + std::to_string(previous) + "\n";

  MPI_Finalize();
  return 0;
}
