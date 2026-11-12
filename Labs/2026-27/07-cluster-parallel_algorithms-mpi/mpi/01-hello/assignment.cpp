/*
A first MPI program. mpirun starts several copies of this program, the
processes: each one runs all of main, with its own variables.

compile with
  mpic++ -Wall -std=c++20 assignment.cpp -o assignment
and run it with 4 processes with
  mpirun -n 4 ./assignment
*/

#include <mpi.h>

#include <iostream>

int main(int argc, char **argv) {
  // The first MPI call of the program
  MPI_Init(&argc, &argv);

  // TODO 1: the number of processes and the rank of this process, with
  //         MPI_Comm_size and MPI_Comm_rank. Both take the communicator
  //         MPI_COMM_WORLD, which contains all the processes, and the address
  //         of the int to write.
  int size = 1;
  int rank = 0;

  // The name of the machine this process runs on
  char name[MPI_MAX_PROCESSOR_NAME];
  int name_length;
  MPI_Get_processor_name(name, &name_length);

  std::cout << "Hello from rank " << rank << " of " << size << ", on " << name << std::endl;

  // TODO 2: only rank 0 prints "Running with <size> processes"

  // TODO 3: each process sums its own part of 1, 2, ..., n.
  //         With 4 processes: rank 0 sums 1, ..., 25, rank 1 sums 26, ..., 50, ...
  //         Print "rank <rank>: sum of <first>, ..., <last> = <sum>".
  const int n = 100;
  const int block = n / size; // how many numbers for each process
  const int first = rank * block + 1;
  const int last = first + block - 1;
  (void)last; // only to avoid the warning "unused variable": remove it

  // TODO 4: the total of the sums of all the processes, on rank 0, with
  //         MPI_Reduce. Then rank 0 prints "total = <total>".
  int total = 0;
  (void)total; // only to avoid the warning "unused variable": remove it

  // The last MPI call of the program
  MPI_Finalize();
  return 0;
}
