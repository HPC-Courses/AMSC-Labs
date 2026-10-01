/*
Standard algorithms + lambdas: a possible solution.

compile with
  g++ -Wall -std=c++20 solution.cpp -o solution
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

  // 1. By reference: the generator must advance its state at every call
  std::vector<double> v(20);
  std::generate(v.begin(), v.end(), [&gen, &dist]() { return dist(gen); });
  print("samples", v);

  // 2.
  double const lo = -1., hi = 1.;
  auto const n_in = std::count_if(v.begin(), v.end(), [lo, hi](double x) { return x >= lo && x <= hi; });
  std::cout << n_in << " samples in [" << lo << ", " << hi << "]" << std::endl;

  // 3. The comparison must be a strict weak ordering: "<", never "<="
  std::sort(v.begin(), v.end(), [](double a, double b) { return std::abs(a) > std::abs(b); });
  print("sorted by |x|", v);

  // 4.
  double const n = static_cast<double>(v.size());
  double const mean = std::accumulate(v.begin(), v.end(), 0.) / n;
  double const var = std::transform_reduce(v.begin(), v.end(), 0., std::plus<>{},
                                           [mean](double x) { return (x - mean) * (x - mean); }) / n;
  double const stddev = std::sqrt(var);
  std::cout << "mean = " << mean << ", stddev = " << stddev << std::endl;

  // 5. Output range == input range: in place
  std::transform(v.begin(), v.end(), v.begin(), [mean, stddev](double x) { return (x - mean) / stddev; });
  print("normalised", v);

  // 6. Before C++20: v.erase(std::remove_if(v.begin(), v.end(), pred), v.end());
  std::erase_if(v, [](double x) { return x < 0.; });
  print("non negative", v);

  // 7. next_id is a data member of the closure; mutable lets operator() modify it
  std::vector<Particle> particles(8);
  std::generate(particles.begin(), particles.end(), [next_id = 0, &gen, &dist]() mutable {
    Particle p{next_id, 1. + next_id, dist(gen)};
    ++next_id;
    return p;
  });

  // 8.
  std::sort(particles.begin(), particles.end(), [](Particle const &a, Particle const &b) { return a.x < b.x; });
  for (auto const &p : particles)
    std::cout << "  id " << p.id << ", mass " << p.mass << ", x " << p.x << std::endl;
  auto it = std::find_if(particles.begin(), particles.end(), [](Particle const &p) { return p.mass > 4.; });
  if (it != particles.end())
    std::cout << "first particle (by position) with mass > 4: id " << it->id << std::endl;

  // 9.
  double const total_mass = std::accumulate(particles.begin(), particles.end(), 0.,
                                            [](double acc, Particle const &p) { return acc + p.mass; });
  double const moment = std::accumulate(particles.begin(), particles.end(), 0.,
                                        [](double acc, Particle const &p) { return acc + p.mass * p.x; });
  std::cout << "centre of mass: " << moment / total_mass << std::endl;
  return 0;
}
