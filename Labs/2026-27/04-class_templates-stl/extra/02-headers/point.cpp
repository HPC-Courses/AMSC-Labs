#include "point.hpp"

#include <cmath>

template <typename T, std::size_t N>
T Point<T, N>::norm() const {
  T s{};
  for (const auto &c : coords_)
    s += c * c;
  return std::sqrt(s);
}
