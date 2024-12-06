#include <mpi.h>

#include <iostream>
#include <string>
#include <vector>


int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);
  MPI_Status status;
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  bool version2 = false;

  // version 2
  if (version2) {
    int rank_s = rank;

    if (rank>0) {
      MPI_Send(&rank_s, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {  // you are process 0
      std::string message = (argc > 1) ? std::string(argv[1]) : "world";
      std::cout << "Hello " + message + ", from rank " + std::to_string(rank_s) << " of size " + std::to_string(size) + "." << std::endl;
      for (int r=1; r<size; ++r)
      {
        MPI_Recv(&rank_s, 1, MPI_INT, r, 0, MPI_COMM_WORLD, &status);
        std::cout << "Hello " + message + ", from rank " + std::to_string(rank_s) << " of size " + std::to_string(size) + "." << std::endl;
      }
    }
  }
  else {
    const std::string greeting = (argc > 1) ? std::string(argv[1]) : "world";
    const std::string message = "Hello " + greeting + ", from rank " + std::to_string(rank) + " of size " + std::to_string(size) + ".";

    if (rank>0) {
      MPI_Send(message.c_str(), message.length(), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    } else {  
      std::cout << message << std::endl;
      char * buf = (char *) malloc(message.length()+1); // create a buffer to receive, a string can just be sent not received
      for (int r=1; r<size; ++r)
      {
        MPI_Recv(buf, message.length(), MPI_CHAR, r, 0, MPI_COMM_WORLD, &status);
        std::cout << buf << std::endl;
      }
      free(buf);
    }

  }

  MPI_Finalize();
  return 0;
}
