/*
A first MPI program. Solution.

compile with
  mpic++ -Wall -std=c++20 solution.cpp -o solution
and run it with 4 processes with
  mpirun -n 4 ./solution
*/

#include <mpi.h>

#include <iostream>

int main(int argc, char **argv) {
  // The first MPI call of the program
  MPI_Init(&argc, &argv);

  // 1. Each process asks how many processes there are, and which one it is
  int size;
  int rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // The name of the machine this process runs on
  char name[MPI_MAX_PROCESSOR_NAME];
  int name_length;
  MPI_Get_processor_name(name, &name_length);

  std::cout << "Hello from rank " << rank << " of " << size << ", on " << name << std::endl;

  // 2. All the processes run this line, only rank 0 prints
  if (rank == 0)
    std::cout << "Running with " << size << " processes" << std::endl;

  // 3. The same code, a different part for each process
  const int n = 100;
  const int block = n / size; // how many numbers for each process
  const int first = rank * block + 1;
  // The last process also takes what is left, when size does not divide n
  // (task 6). With "first + block - 1" for all the processes, the last
  // numbers are summed by nobody.
  const int last = (rank == size - 1) ? n : first + block - 1;

  int sum = 0;
  for (int k = first; k <= last; ++k)
    sum += k;
  std::cout << "rank " << rank << ": sum of " << first << ", ..., " << last << " = " << sum << std::endl;

  // 4. Each process sends its sum, rank 0 receives their total.
  //    All the processes must call MPI_Reduce, not only rank 0.
  int total = 0;
  MPI_Reduce(&sum, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  if (rank == 0)
    std::cout << "total = " << total << std::endl;

  // The last MPI call of the program
  MPI_Finalize();
  return 0;
}
