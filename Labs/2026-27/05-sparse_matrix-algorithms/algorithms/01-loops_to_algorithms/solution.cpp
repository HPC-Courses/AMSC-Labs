/*
From a loop to an algorithm.
Solution.

compile with
  g++ -Wall -std=c++20 solution.cpp -o solution
and run with
  ./solution
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
  // The initial value is 0., not 0: its type is the type of the sum.
  // With 0 the sum is an int, and -1.5 is truncated.
  double sum = std::accumulate(x.begin(), x.end(), 0.);
  check1(sum);

  // ---------------------------------------------------------------------------
  // EXERCISE 2
  // The second vector is given only by its beginning: it must have at least
  // as many entries as the first one.
  double dot = std::inner_product(x.begin(), x.end(), y.begin(), 0.);
  check2(dot);

  // ---------------------------------------------------------------------------
  // EXERCISE 3
  const double a = 2.;
  std::vector<double> z(x.size());
  std::transform(x.begin(), x.end(), y.begin(), z.begin(),
                 [a](double xi, double yi) { return a * xi + yi; });
  check3(z);

  // ---------------------------------------------------------------------------
  // EXERCISE 4
  auto absolute_value = [](double v) { return std::abs(v); };
  double norm1 = std::transform_reduce(x.begin(), x.end(), 0., std::plus<double>(), absolute_value);
  check4(norm1);

  // ---------------------------------------------------------------------------
  // EXERCISE 5
  auto is_large = [](double v) { return std::abs(v) > 2.5; };
  std::size_t how_many = std::count_if(x.begin(), x.end(), is_large);
  std::vector<double> large;
  large.reserve(how_many); // large is still empty, but it will not reallocate
  std::copy_if(x.begin(), x.end(), std::back_inserter(large), is_large);
  check5(how_many, large);

  // ---------------------------------------------------------------------------
  // EXERCISE 6
  // The lambda answers the question "is u smaller than v?"
  auto smaller_in_absolute_value = [](double u, double v) { return std::abs(u) < std::abs(v); };
  auto it = std::max_element(x.begin(), x.end(), smaller_in_absolute_value);
  double largest = *it;
  std::size_t position = std::distance(x.begin(), it);
  check6(largest, position);

  return 0;
}
