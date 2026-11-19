/*
A ring: each process sends a message to the next one, rank + 1, and receives
one from the previous one, rank - 1. The last process sends to rank 0.
Solution.

compile with
  mpic++ -Wall -std=c++20 solution.cpp -o solution
and run it with 4 processes, and messages of m numbers, with
  mpirun -n 4 ./solution 1
  mpirun -n 4 ./solution 100000
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

  // 1. All the processes send first, then receive. MPI_Send may wait until
  //    the message is received: for short messages Open MPI copies it into a
  //    buffer and returns at once (in the container: 1000 int yes, 1024 no),
  //    for long ones it waits for the matching MPI_Recv.
  //    Then all the processes wait in MPI_Send, nobody reaches MPI_Recv:
  //    a deadlock.
  //
  //   MPI_Send(message.data(), m, MPI_INT, next, 0, MPI_COMM_WORLD);
  //   MPI_Recv(received.data(), m, MPI_INT, previous, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  // 2. MPI_Sendrecv sends and receives at the same time, and MPI makes sure
  //    that the ring does not block, whatever the size of the message.
  MPI_Sendrecv(message.data(), m, MPI_INT, next, 0,      // what we send, and to whom
               received.data(), m, MPI_INT, previous, 0, // what we receive, and from whom
               MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  std::cout << "rank " + std::to_string(rank) + ": received " + std::to_string(received[0]) +
                   " from rank " + std::to_string(previous) + "\n";

  MPI_Finalize();
  return 0;
}
