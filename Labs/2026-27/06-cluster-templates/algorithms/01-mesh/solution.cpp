/*
The nodes of a 1D mesh, handled with the algorithms of the standard library. Solution.

compile with
  g++ -Wall -std=c++20 solution.cpp -o solution
and run with
  ./solution
*/

#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

#include "check.hpp"

// EXERCISE 4
// upper_bound returns the first node larger than x: the right end of the element
std::size_t element_of(const std::vector<double> &nodes, double x) {
  const auto right = std::upper_bound(nodes.begin(), nodes.end(), x);
  return std::distance(nodes.begin(), right) - 1;
}

int main() {
  // The nodes of two meshes of [0, 1], put together: not sorted, and some are repeated
  std::vector<double> nodes = {0.5, 0., 0.75, 1., 0.25, 0.5, 0.875, 1., 0.75};

  // EXERCISE 1
  std::sort(nodes.begin(), nodes.end());
  const auto new_end = std::unique(nodes.begin(), nodes.end());
  nodes.erase(new_end, nodes.end());
  check1(nodes);

  // EXERCISE 2
  // The first number written is nodes[0] itself, not a difference: we remove it
  std::vector<double> h(nodes.size());
  std::adjacent_difference(nodes.begin(), nodes.end(), h.begin());
  h.erase(h.begin());
  check2(h);

  // EXERCISE 3
  const auto [smallest, largest] = std::minmax_element(h.begin(), h.end());
  const double hmin = *smallest;
  const double hmax = *largest;
  check3(hmin, hmax);

  // EXERCISE 4
  check4({element_of(nodes, 0.1), element_of(nodes, 0.5), element_of(nodes, 0.8)});

  // EXERCISE 5
  std::vector<double> midpoints(nodes.size());
  std::vector<double> refined;
  std::adjacent_difference(nodes.begin(), nodes.end(), midpoints.begin(),
                           [](double right, double left) { return 0.5 * (left + right); });
  midpoints.erase(midpoints.begin());
  std::merge(nodes.begin(), nodes.end(), midpoints.begin(), midpoints.end(), std::back_inserter(refined));
  check5(refined);

  return 0;
}
