#ifndef POINT_HPP
#define POINT_HPP

#include <array>
#include <cstddef>

template <typename T, std::size_t N>
class Point {
public:
  explicit Point(const std::array<T, N> &coords) : coords_{coords} {}

  // Declared here, defined in point.cpp, as we do for ordinary classes.
  // Is it a good idea for a template?
  T norm() const;

private:
  std::array<T, N> coords_;
};

#endif /* POINT_HPP */
