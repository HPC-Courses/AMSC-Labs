#include <mpi.h>
#include <iostream>

class MPI_Manager {
 public:
  MPI_Manager(MPI_Manager const&) = delete;
  void operator=(MPI_Manager const&) = delete;
  static MPI_Manager& initialize(int* argc, char*** argv) {
    // Guaranteed to be destroyed. Instantiated on first use.
    static MPI_Manager instance(argc, argv);
    return instance;
  }
  static int getWorldRank() {
    return rank();
  }
  static int getWorldSize() {
    return size();
  }
  ~MPI_Manager() { MPI_Finalize(); }

 private:
  MPI_Manager(int* argc, char*** argv) {
    MPI_Init(argc, argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank());
    MPI_Comm_size(MPI_COMM_WORLD, &size());
  };
  static int& rank() {
    static int rank;
    return rank;
  }
  static int& size() {
    static int size;
    return size;
  }
};

void some_function() {
  std::cout << "Hello world from rank " << MPI_Manager::getWorldRank()
            << " out of " << MPI_Manager::getWorldSize() << " processors" << std::endl;
}

int main(int argc, char* argv[]) {
  MPI_Manager::initialize(&argc, &argv);
  some_function();
  
  return 0;
}