/*
A class template is a family of classes.

compile with
  g++ -Wall -std=c++20 point.cpp -o point
and run with
  ./point

Before running, try to predict the output of points 2, 3 and 4!
*/

#include <array>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <string>
#include <type_traits>

// A point in N dimensions, with coordinates of type T.
// - T is a type template parameter;
// - N is a non-type template parameter: a value known at compile time.
//   It has a default: Point<double> is Point<double, 3>.
template <typename T, std::size_t N = 3>
class Point {
public:
  Point() = default; // all the coordinates are zero
  explicit Point(const std::array<T, N> &coords) : coords_{coords} {}

  T &operator[](std::size_t i) { return coords_[i]; }
  const T &operator[](std::size_t i) const { return coords_[i]; }
  static constexpr std::size_t dim() { return N; }

  // Euclidean norm. It needs T * T and std::sqrt(T): the compiler generates
  // it only for the types for which somebody calls it
  T norm() const {
    T s{};
    for (const auto &c : coords_)
      s += c * c;
    return std::sqrt(s);
  }

  void print() const; // defined outside the class, below

private:
  std::array<T, N> coords_{}; // no heap: N is known at compile time
};

// Definition outside the class: the template parameter list is repeated,
// and the name of the class is Point<T, N>
template <typename T, std::size_t N>
void Point<T, N>::print() const {
  std::cout << "  (";
  for (std::size_t i = 0; i < N; ++i)
    std::cout << coords_[i] << (i + 1 < N ? ", " : ")");
  std::cout << std::endl;
}

// Alias template: a shorter name for a family of types, NOT a new type
template <typename T>
using Point2D = Point<T, 2>;

int main() {
  std::cout << std::boolalpha;

  std::cout << "1. instantiation" << std::endl;
  Point<double, 2> p{{3., 4.}};
  Point<double> q; // Point<double, 3>
  p.print();
  q.print();
  std::cout << "  |p| = " << p.norm() << ", q has " << q.dim() << " coordinates" << std::endl;

  std::cout << "2. different arguments, different types" << std::endl;
  std::cout << "  Point<double, 2> is Point<double, 3>? "
            << std::is_same_v<Point<double, 2>, Point<double, 3>> << std::endl;
  std::cout << "  Point2D<double>  is Point<double, 2>? "
            << std::is_same_v<Point2D<double>, Point<double, 2>> << std::endl;
  // q = p; // try it

  std::cout << "3. class template argument deduction (CTAD)" << std::endl;
  Point r{std::array{1.f, 2.f, 2.f}}; // which T? which N?
  r.print();
  std::cout << "  |r| = " << r.norm() << ", sizeof(r) = " << sizeof(r) << " bytes" << std::endl;

  std::cout << "4. integer coordinates" << std::endl;
  Point2D<int> a{{3, 4}};
  Point2D<int> b{{1, 1}};
  std::cout << "  |a| = " << a.norm() << ", |b| = " << b.norm() << std::endl;

  std::cout << "5. a member is generated only if it is used" << std::endl;
  Point2D<std::string> names{{"x", "y"}};
  names.print();
  // std::cout << names.norm() << std::endl; // try it
  return 0;
}
