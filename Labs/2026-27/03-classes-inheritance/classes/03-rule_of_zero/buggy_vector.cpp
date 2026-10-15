/*
A vector of doubles that manages its memory by hand... and has a bug.

compile with AddressSanitizer:
  g++ -Wall -std=c++20 -g -fsanitize=address buggy_vector.cpp -o buggy_vector
and run with
  ./buggy_vector
*/

#include <cstddef>
#include <iostream>

class Vector {
public:
  explicit Vector(std::size_t n) : size_{n}, data_{new double[n]{}} {}
  ~Vector() { delete[] data_; }

  std::size_t size() const { return size_; }
  double &operator[](std::size_t i) { return data_[i]; }
  const double &operator[](std::size_t i) const { return data_[i]; }

private:
  std::size_t size_;
  double *data_; // owning raw pointer: the compiler-generated copy copies the ADDRESS
};

double sum(Vector v) { // by value: a copy!
  double s = 0.;
  for (std::size_t i = 0; i < v.size(); ++i)
    s += v[i];
  return s;
}

int main() {
  Vector a(5);
  for (std::size_t i = 0; i < a.size(); ++i)
    a[i] = static_cast<double>(i);

  Vector b = a; // copy
  b[0] = 100.;
  std::cout << "a[0] = " << a[0] << " (expected 0)" << std::endl;

  std::cout << "sum(a) = " << sum(a) << std::endl;
  std::cout << "a[1] = " << a[1] << " (expected 1)" << std::endl;
  return 0;
}
