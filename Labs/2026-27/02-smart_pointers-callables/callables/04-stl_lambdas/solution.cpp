/*
Standard algorithms + lambdas (homework): a possible solution.

compile with
  g++ -Wall -std=c++20 solution.cpp -o solution
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

  // 1. The bounds are captured by value
  double const lo = -1., hi = 1.;
  auto n = std::count_if(v.begin(), v.end(), [lo, hi](double x) { return lo <= x && x <= hi; });
  std::cout << n << " values in [" << lo << ", " << hi << "]" << std::endl;

  // 2. The comparison must be strict (>, never >=)
  std::sort(v.begin(), v.end(), [](double a, double b) { return std::abs(a) > std::abs(b); });
  print("sorted by |x|", v);

  // 3. No lambda needed: the default operation of std::accumulate is +
  double const mean = std::accumulate(v.begin(), v.end(), 0.) / static_cast<double>(v.size());
  std::cout << "mean = " << mean << std::endl;

  // 4. The output range is v itself: in place
  std::transform(v.begin(), v.end(), v.begin(), [mean](double x) { return x - mean; });
  print("minus the mean", v);

  // 5.
  std::erase_if(v, [](double x) { return x < 0.; });
  print("non negative", v);

  // 6. Optional
  std::vector<Particle> particles{
      {0, 1., 0.3}, {1, 2., -1.2}, {2, 2., 0.8}, {3, 7., 2.5}, {4, 3., -0.5}, {5, 6., 1.1}};
  std::sort(particles.begin(), particles.end(),
            [](Particle const &a, Particle const &b) { return a.x < b.x; });
  auto it = std::find_if(particles.begin(), particles.end(),
                         [](Particle const &p) { return p.mass > 4.; });
  if (it != particles.end())
    std::cout << "first particle with mass > 4: id " << it->id << ", x = " << it->x << std::endl;

  return 0;
}
