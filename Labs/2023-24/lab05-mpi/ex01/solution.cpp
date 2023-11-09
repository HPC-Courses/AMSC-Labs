#include <mpi.h>

#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const std::string greeting = (argc > 1) ? argv[1] : "world";
  const std::string message = "Hello " + greeting + ", from rank " +
                              std::to_string(rank) + " of " +
                              std::to_string(size);

  if (rank > 0) {
    const unsigned length = unsigned(message.size());
    MPI_Send(&length, 1, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD);
    MPI_Send(&message[0], length, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
  } else {
    std::cout << message << std::endl;
    std::string recv_message;
    unsigned recv_length;
    for (int r = 1; r < size; r++) {
      MPI_Recv(&recv_length, 1, MPI_UNSIGNED, r, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      recv_message.resize(recv_length);
      MPI_Recv(&recv_message[0], recv_length, MPI_CHAR, r, 1, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      std::cout << recv_message << std::endl;
    }
  }

  MPI_Finalize();
  return 0;
}