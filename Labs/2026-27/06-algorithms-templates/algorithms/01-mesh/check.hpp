#ifndef CHECK_HPP
#define CHECK_HPP

// Compares your results with the expected ones. Do not modify this file.
// All the expected values are multiples of 1/16, exactly representable as double:
// comparing them with == is safe here.

#include <array>
#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

// Numbers, vectors and pairs all have operator==
template <typename T>
void check(int exercise, const T &result, const T &expected) {
  std::cout << "Exercise " << exercise << ": " << (result == expected ? "PASSED" : "FAILED") << std::endl;
}

inline void check1(const std::vector<double> &nodes) {
  check(1, nodes, {0., 0.25, 0.5, 0.75, 0.875, 1.});
}

inline void check2(const std::vector<double> &h) { check(2, h, {0.25, 0.25, 0.25, 0.125, 0.125}); }

inline void check3(double hmin, double hmax) { check(3, std::pair{hmin, hmax}, std::pair{0.125, 0.25}); }

inline void check4(const std::array<std::size_t, 3> &elements) { check(4, elements, {0, 2, 3}); }

inline void check5(const std::vector<double> &refined) {
  check(5, refined,
        {0., 0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.8125, 0.875, 0.9375, 1.});
}

#endif /* CHECK_HPP */
