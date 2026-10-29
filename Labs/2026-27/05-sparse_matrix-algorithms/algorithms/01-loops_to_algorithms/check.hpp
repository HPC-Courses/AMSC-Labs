#ifndef CHECK_HPP
#define CHECK_HPP

// Compares your results with the expected ones. Do not modify this file.
// All the expected values are small numbers, exactly representable as double:
// comparing them with == is safe here.

#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

// Numbers, vectors and pairs all have operator==
template <typename T>
void check(int exercise, const T &result, const T &expected) {
  std::cout << "Exercise " << exercise << ": " << (result == expected ? "PASSED" : "FAILED") << std::endl;
}

inline void check1(double sum) { check(1, sum, 8.5); }

inline void check2(double dot) { check(2, dot, 40.); }

inline void check3(const std::vector<double> &z) {
  check(3, z, {7., 0., 11., 1., 15., -12., 11., 20.});
}

inline void check4(double norm1) { check(4, norm1, 31.5); }

inline void check5(std::size_t how_many, const std::vector<double> &large) {
  check(5, std::pair{how_many, large}, std::pair{std::size_t{5}, std::vector<double>{3., 4., 5., -9., 6.}});
}

inline void check6(double largest, std::size_t position) {
  check(6, std::pair{largest, position}, std::pair{-9., std::size_t{5}});
}

#endif /* CHECK_HPP */
