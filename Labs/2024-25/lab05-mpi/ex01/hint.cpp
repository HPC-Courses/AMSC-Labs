#include <mpi.h>

#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const std::string greeting =
      /*more than one argument*/ ? /*second string passed with argv*/
                                 : /*default greeting*/;
  const std::string message = /*assemble the message*/;

  if (rank) {
    // send your local message size and then message to processor 0
  } else {  // you are process 0
    // output your message
    // receive all the messages from the other processors and output them
  }

  MPI_Finalize();
  return 0;
}