/*
std::reduce and std::transform_reduce: the algorithms that combine all the
elements into one result, and that have a parallel version.
For now they run on one thread: section 2.1 shows how to ask for several threads.
Each exercise shows the solution of Lab 5, in a comment: write the same
computation with the algorithm that is suggested. check.hpp tells you if the
result is correct.

compile with
  g++ -Wall -std=c++20 assignment.cpp -o assignment
and run with
  ./assignment
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

  // ---------------------------------------------------------------------------
  // EXERCISE 1
  // The sum of the entries of x. In Lab 5:
  //   double sum = std::accumulate(x.begin(), x.end(), 0.);
  // Write it with std::reduce (in <numeric>). The arguments are the same.
  double sum = 0.;
  // TODO
  check1(sum);

  // ---------------------------------------------------------------------------
  // EXERCISE 2
  // The dot product of x and y. In Lab 5:
  //   double dot = std::inner_product(x.begin(), x.end(), y.begin(), 0.);
  // Write it with std::transform_reduce (in <numeric>), the version that takes
  // two vectors: it multiplies x[i] by y[i], then sums. The arguments are the same.
  double dot = 0.;
  // TODO
  check2(dot);

  // ---------------------------------------------------------------------------
  // EXERCISE 3
  // The largest entry of x. With a loop:
  //   double largest = x[0];
  //   for (double v : x)
  //     largest = std::max(largest, v);
  // Write it with std::reduce, with x[0] as initial value and, as the last
  // argument, a lambda that takes two doubles and returns the larger one.
  double largest = 0.;
  // TODO
  check3(largest);

  // ---------------------------------------------------------------------------
  // NOW BREAK IT
  // The same algorithm, with std::minus instead of std::plus:
  // 0 - x[0] - x[1] - ... - x[7] = -8.5
  const double minus_accumulate = std::accumulate(x.begin(), x.end(), 0., std::minus<double>());
  // TODO: uncomment the two lines below. Are the two results the same?
  // const double minus_reduce = std::reduce(x.begin(), x.end(), 0., std::minus<double>());
  // std::cout << "accumulate: " << minus_accumulate << ", reduce: " << minus_reduce << std::endl;
  (void)minus_accumulate; // only to avoid the warning "unused variable": remove it

  return 0;
}
