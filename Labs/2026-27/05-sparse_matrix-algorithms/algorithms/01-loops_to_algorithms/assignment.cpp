/*
From a loop to an algorithm.
Each exercise shows a loop, in a comment: write the same computation with the
algorithm that is suggested. check.hpp tells you if the result is correct.

compile with
  g++ -Wall -std=c++20 assignment.cpp -o assignment
and run with
  ./assignment
*/

#include <algorithm>
#include <cmath>
#include <functional>
#include <iterator>
#include <numeric>
#include <vector>

#include "check.hpp"

int main() {
  const std::vector<double> x = {3., -1., 4., -1.5, 5., -9., 2., 6.};
  const std::vector<double> y = {1., 2., 3., 4., 5., 6., 7., 8.};

  // ---------------------------------------------------------------------------
  // EXERCISE 1
  // The sum of the entries of x. With a loop:
  //   double sum = 0.;
  //   for (double v : x)
  //     sum += v;
  // Write it with std::accumulate (in <numeric>).
  double sum = 0.;
  // TODO
  check1(sum);

  // ---------------------------------------------------------------------------
  // EXERCISE 2
  // The dot product of x and y. With a loop:
  //   double dot = 0.;
  //   for (std::size_t i = 0; i < x.size(); ++i)
  //     dot += x[i] * y[i];
  // Write it with std::inner_product (in <numeric>).
  double dot = 0.;
  // TODO
  check2(dot);

  // ---------------------------------------------------------------------------
  // EXERCISE 3
  // z = a x + y, with a = 2. With a loop:
  //   for (std::size_t i = 0; i < x.size(); ++i)
  //     z[i] = a * x[i] + y[i];
  // Write it with the std::transform that takes two vectors (slide 88).
  // z already has the right size: the algorithm writes into it, it does not add elements.
  const double a = 2.;
  std::vector<double> z(x.size());
  // TODO
  check3(z);

  // ---------------------------------------------------------------------------
  // EXERCISE 4
  // The 1-norm of x, the sum of the absolute values. With a loop:
  //   double norm1 = 0.;
  //   for (double v : x)
  //     norm1 += std::abs(v);
  // Write it with std::transform_reduce (slide 90): it first transforms each
  // entry (here: its absolute value), then sums the results (std::plus<double>()).
  double norm1 = 0.;
  // TODO
  check4(norm1);

  // ---------------------------------------------------------------------------
  // EXERCISE 5
  // Copy into large the entries of x larger than 2.5 in absolute value. With a loop:
  //   for (double v : x)
  //     if (std::abs(v) > 2.5)
  //       large.push_back(v);
  // Write it in two steps (slide 86):
  //  - count them with std::count_if, and reserve the space in large;
  //  - copy them with std::copy_if and std::back_inserter(large).
  // Write the condition once, in a lambda, and use it in both algorithms.
  std::size_t how_many = 0;
  std::vector<double> large;
  // TODO
  check5(how_many, large);

  // ---------------------------------------------------------------------------
  // EXERCISE 6 (if you have time)
  // The entry of x with the largest absolute value, and its position. With a loop:
  //   std::size_t position = 0;
  //   for (std::size_t i = 1; i < x.size(); ++i)
  //     if (std::abs(x[i]) > std::abs(x[position]))
  //       position = i;
  //   double largest = x[position];
  // Write it with std::max_element and a lambda that compares two entries by
  // their absolute value. max_element returns an iterator: std::distance
  // (slide 21) tells you its position.
  double largest = 0.;
  std::size_t position = 0;
  // TODO
  check6(largest, position);

  return 0;
}
