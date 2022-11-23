#include <omp.h>

#include <iostream>

int main(int argc, char **argv) {
#pragma omp parallel
  {
    int thread_id = omp_get_thread_num();
    std::cout << "Hello World from thread " << thread_id << "!" << std::endl;
  }

  return 0;
}