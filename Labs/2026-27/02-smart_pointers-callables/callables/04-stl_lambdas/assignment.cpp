/*
Standard algorithms + lambdas (homework). Fill in the TODOs, using one
algorithm of <algorithm> or <numeric> and one lambda for each of them:
no for loops, except for printing.

compile with
  g++ -Wall -std=c++20 assignment.cpp -o assignment
*/

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

struct Particle {
  int id;
  double mass;
  double x; // position
};

void print(std::string const &title, std::vector<double> const &v) {
  std::cout << title << ":";
  for (double x : v)
    std::cout << " " << x;
  std::cout << std::endl;
}

int main() {
  std::vector<double> v{0.5, -1.2, 2.3, -0.4, 1.8, -2.1, 0.9, 0.1, -0.7, 1.5};
  print("values", v);

  // 1. Count the values in [-1, 1] (std::count_if). Make the bounds two local
  //    variables, lo and hi, and capture them
  // TODO

  // 2. Sort v by decreasing absolute value (std::sort with a comparison)
  // TODO
  print("sorted by |x|", v);

  // 3. Compute the mean of v (std::accumulate)
  // TODO

  // 4. Subtract the mean from every value, in place (std::transform).
  //    How do you capture the mean?
  // TODO
  print("minus the mean", v);

  // 5. Remove the negative values (std::erase_if, C++20)
  // TODO
  print("non negative", v);

  // ---- 6. Optional ----
  std::vector<Particle> particles{
      {0, 1., 0.3}, {1, 2., -1.2}, {2, 2., 0.8}, {3, 7., 2.5}, {4, 3., -0.5}, {5, 6., 1.1}};
  // Sort the particles by position, then find the first one with mass > 4
  // (std::find_if) and print its id
  // TODO

  return 0;
}
