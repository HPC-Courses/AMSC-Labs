/*
Standard algorithms + lambdas. Fill in the TODOs, using an algorithm of
<algorithm> or <numeric> and a lambda for each of them: no raw for loops,
except for printing.

compile with
  g++ -Wall -std=c++20 assignment.cpp -o assignment
*/

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
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
  std::mt19937 gen{42};
  std::normal_distribution<double> dist{0., 1.};

  // 1. Fill v with 20 samples of dist (std::generate). The lambda must capture gen and dist
  std::vector<double> v(20);
  // TODO
  print("samples", v);

  // 2. Count the samples in [-1, 1] (std::count_if). Make the bounds two local
  //    variables and capture them
  // TODO

  // 3. Sort v by decreasing absolute value (std::sort with a comparison)
  // TODO
  print("sorted by |x|", v);

  // 4. Compute mean and standard deviation (std::accumulate / std::transform_reduce)
  // TODO

  // 5. Normalise v in place: x -> (x - mean) / stddev (std::transform)
  // TODO
  print("normalised", v);

  // 6. Remove all the negative values (std::erase_if, C++20)
  // TODO
  print("non negative", v);

  // ---- Points 7-9: at home ----

  // 7. A vector of particles with ids 0,1,2,... : use std::generate with a
  //    *mutable* lambda that keeps the next id in its own capture
  std::vector<Particle> particles(8);
  // TODO: id = next id, mass = 1 + id, x = sample of dist

  // 8. Sort the particles by position, then find the first one with mass > 4
  //    (std::find_if) and print its id
  // TODO

  // 9. Compute the centre of mass sum(m_i x_i) / sum(m_i)
  // TODO

  return 0;
}
