/*
Solution.
std::reduce and std::transform_reduce: the algorithms that combine all the
elements into one result, and that have a parallel version.
For now they run on one thread: section 2.1 shows how to ask for several threads.

compile with
  g++ -Wall -std=c++20 solution.cpp -o solution
and run with
  ./solution
*/

#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

#include "check.hpp"

int main() {
  // The same vectors as in Lab 5
  const std::vector<double> x = {3., -1., 4., -1.5, 5., -9., 2., 6.};
  const std::vector<double> y = {1., 2., 3., 4., 5., 6., 7., 8.};

  // EXERCISE 1
  // As in accumulate, the type of the initial value is the type of the sum: 0., not 0
  const double sum = std::reduce(x.begin(), x.end(), 0.);
  check1(sum);

  // EXERCISE 2
  // By default, transform_reduce multiplies (std::multiplies) and sums (std::plus)
  const double dot = std::transform_reduce(x.begin(), x.end(), y.begin(), 0.);
  check2(dot);

  // EXERCISE 3
  // std::max is associative and commutative: the order does not matter
  auto larger = [](double a, double b) { return std::max(a, b); };
  const double largest = std::reduce(x.begin(), x.end(), x[0], larger);
  check3(largest);

  // NOW BREAK IT
  // accumulate subtracts from left to right. reduce may group the elements in
  // any order, and with std::minus the order changes the result
  const double minus_accumulate = std::accumulate(x.begin(), x.end(), 0., std::minus<double>());
  const double minus_reduce = std::reduce(x.begin(), x.end(), 0., std::minus<double>());
  std::cout << "accumulate: " << minus_accumulate << ", reduce: " << minus_reduce << std::endl;

  return 0;
}
