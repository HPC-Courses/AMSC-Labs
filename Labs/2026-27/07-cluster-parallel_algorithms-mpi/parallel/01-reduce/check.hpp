#ifndef CHECK_HPP
#define CHECK_HPP

// Compares your results with the expected ones. Do not modify this file.
// All the expected values are small numbers, exactly representable as double:
// comparing them with == is safe here.

#include <iostream>

inline void check(int exercise, double result, double expected) {
  std::cout << "Exercise " << exercise << ": " << (result == expected ? "PASSED" : "FAILED") << std::endl;
}

inline void check1(double sum) { check(1, sum, 8.5); }

inline void check2(double dot) { check(2, dot, 40.); }

inline void check3(double largest) { check(3, largest, 6.); }

#endif /* CHECK_HPP */
