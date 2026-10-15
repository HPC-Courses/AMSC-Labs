/*
The same Vector, following the rule of zero: the data are stored in a
std::vector, which already knows how to copy, move and destroy itself.
The compiler-generated special member functions are therefore correct,
and we do not write any of them.

compile with
  g++ -Wall -std=c++20 -g -fsanitize=address vector.cpp -o vector
*/

#include <cstddef>
#include <iostream>
#include <vector>

class Vector {
public:
  explicit Vector(std::size_t n) : data_(n) {} // parentheses: n elements, not {n}!

  std::size_t size() const { return data_.size(); }
  double &operator[](std::size_t i) { return data_[i]; }
  const double &operator[](std::size_t i) const { return data_[i]; }

private:
  std::vector<double> data_;
};

double sum(const Vector &v) { // no reason to copy
  double s = 0.;
  for (std::size_t i = 0; i < v.size(); ++i)
    s += v[i];
  return s;
}

int main() {
  Vector a(5);
  for (std::size_t i = 0; i < a.size(); ++i)
    a[i] = static_cast<double>(i);

  Vector b = a; // a real (deep) copy
  b[0] = 100.;
  std::cout << "a[0] = " << a[0] << " (expected 0)" << std::endl;

  std::cout << "sum(a) = " << sum(a) << std::endl;
  std::cout << "a[1] = " << a[1] << " (expected 1)" << std::endl;
  return 0;
}
