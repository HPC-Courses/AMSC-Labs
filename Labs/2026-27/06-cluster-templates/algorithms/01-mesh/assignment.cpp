/*
The nodes of a 1D mesh, handled with the algorithms of the standard library.
Each exercise says what to compute and which algorithm to use, and the first
ones show the loop that the algorithm replaces. Write your solution where you
find TODO: check.hpp tells you if it is correct.

compile with
  g++ -Wall -std=c++20 assignment.cpp -o assignment
and run with
  ./assignment
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
// The element i of the mesh is [nodes[i], nodes[i + 1]). Return the index of the
// element that contains x, with std::upper_bound and std::distance.
// nodes is sorted: this is a binary search, in O(log n).
std::size_t element_of(const std::vector<double> &nodes, double x) {
  (void)nodes, (void)x; // remove this line when you use them
  // TODO
  return 0;
}

int main() {
  // The nodes of two meshes of [0, 1], put together: not sorted, and some are repeated
  std::vector<double> nodes = {0.5, 0., 0.75, 1., 0.25, 0.5, 0.875, 1., 0.75};

  // ---------------------------------------------------------------------------
  // EXERCISE 1
  // Sort nodes, then remove the duplicates with std::unique.
  // Careful: std::unique does not change the size of the vector. It returns an
  // iterator to the new end: erase what is left, from there to nodes.end().
  // TODO
  check1(nodes);

  // ---------------------------------------------------------------------------
  // EXERCISE 2
  // The sizes of the elements:
  //   for (std::size_t i = 0; i + 1 < nodes.size(); ++i)
  //     h.push_back(nodes[i + 1] - nodes[i]);
  // Write it with std::adjacent_difference (in <numeric>), writing into h.
  // Careful: which is the first number that it writes?
  std::vector<double> h(nodes.size());
  // TODO
  check2(h);

  // ---------------------------------------------------------------------------
  // EXERCISE 3
  // The smallest and the largest size, with one call to std::minmax_element.
  // It returns a pair of iterators: auto [smallest, largest] = ...
  double hmin = 0.;
  double hmax = 0.;
  // TODO
  check3(hmin, hmax);

  // ---------------------------------------------------------------------------
  // EXERCISE 4
  // Write element_of, at the top of this file
  check4({element_of(nodes, 0.1), element_of(nodes, 0.5), element_of(nodes, 0.8)});

  // ---------------------------------------------------------------------------
  // EXERCISE 5 (if you have time)
  // Refine the mesh: add the midpoint of each element.
  // std::adjacent_difference takes one more argument, the operation that replaces
  // the difference: use it to compute the midpoints, then remove the first number
  // as in exercise 2. Both ranges are now sorted: std::merge writes them, sorted,
  // into refined, through a std::back_inserter.
  std::vector<double> midpoints(nodes.size());
  std::vector<double> refined;
  // TODO
  check5(refined);

  return 0;
}
