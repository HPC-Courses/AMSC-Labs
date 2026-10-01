/*
Three classic bugs of manual memory management.

Compile once without and once with AddressSanitizer:
  g++ -Wall -std=c++20 raw_pointers.cpp -o raw_pointers
  g++ -Wall -std=c++20 -g -fsanitize=address raw_pointers.cpp -o raw_pointers_asan

and run each case:
  ./raw_pointers leak      ./raw_pointers_asan leak
  ./raw_pointers double    ./raw_pointers_asan double
  ./raw_pointers dangling  ./raw_pointers_asan dangling

Without the sanitizer the program often *seems* to work: that is exactly the problem.
*/

#include <iostream>
#include <stdexcept>
#include <string>

// Returns a dynamically allocated array: who is responsible for deleting it?
double *make_data(std::size_t n) {
  double *p = new double[n];
  for (std::size_t i = 0; i < n; ++i)
    p[i] = static_cast<double>(i);
  return p;
}

double checked_sum(double const *data, std::size_t n) {
  double s = 0.;
  for (std::size_t i = 0; i < n; ++i) {
    if (data[i] > 5.)
      throw std::runtime_error("value out of range");
    s += data[i];
  }
  return s;
}

// 1. Memory leak: the exception skips the delete[]
void leak() {
  double *data = make_data(10);
  try {
    double s = checked_sum(data, 10); // throws
    std::cout << "sum = " << s << std::endl;
    delete[] data; // never reached
  } catch (std::exception const &e) {
    std::cout << "caught: " << e.what() << std::endl;
  }
}

// 2. Double deletion: two raw pointers "own" the same object
void double_delete() {
  double *a = make_data(10);
  double *b = a; // b is a copy of the address, not of the data
  delete[] a;
  delete[] b; // undefined behaviour
}

// 3. Dangling pointer: the object is gone, the address is still there
void dangling() {
  double *data = make_data(10);
  double *third = &data[2];
  delete[] data;
  std::cout << "third = " << *third << std::endl; // use after free
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " leak|double|dangling" << std::endl;
    return 1;
  }
  std::string const which = argv[1];
  if (which == "leak")
    leak();
  else if (which == "double")
    double_delete();
  else if (which == "dangling")
    dangling();
  else {
    std::cerr << "Unknown case " << which << std::endl;
    return 1;
  }
  std::cout << "end of main" << std::endl;
  return 0;
}
