/*
The same four algorithms, run with three execution policies:
  seq        one thread, one element after the other (as with no policy)
  par        several threads
  par_unseq  several threads, and each thread may also use vector instructions

compile with
  g++ -Wall -std=c++20 -O3 assignment.cpp -o assignment -ltbb
and run with
  ./assignment

-ltbb: with g++, the parallel algorithms create their threads with the library
TBB (Threading Building Blocks), so we must link it.
*/

#include <algorithm>
#include <chrono>
#include <cmath>
#include <execution>
#include <functional>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <thread>
#include <vector>

using Clock = std::chrono::steady_clock;

// Milliseconds elapsed since start
double ms_since(Clock::time_point start) {
  return std::chrono::duration<double, std::milli>(Clock::now() - start).count();
}

// Runs four algorithms with the execution policy `policy`, and prints how long
// each one takes. seq, par and par_unseq are objects of three different types:
// this is why the policy is a template parameter, as the callables of Lab 2.
template <typename Policy>
void run(const std::string &name, Policy policy, const std::vector<double> &x) {
  std::vector<double> y(x.size());
  std::cout << name << std::endl;

  // 1. y = 2 x + 1: very little work for each element
  auto start = Clock::now();
  // TODO: add policy as the first argument, here and in the three algorithms below
  std::transform(x.begin(), x.end(), y.begin(), [](double v) { return 2. * v + 1.; });
  std::cout << "  y = 2 x + 1:          " << ms_since(start) << " ms" << std::endl;

  // 2. y = sin(x) exp(-x / 10): a lot of work for each element
  start = Clock::now();
  std::transform(x.begin(), x.end(), y.begin(), [](double v) { return std::sin(v) * std::exp(-v / 10.); });
  std::cout << "  y = sin(x) exp(-x/10): " << ms_since(start) << " ms" << std::endl;

  // 3. The sum of the squares of x
  start = Clock::now();
  auto square = [](double v) { return v * v; };
  double sum = std::transform_reduce(x.begin(), x.end(), 0., std::plus<double>(), square);
  // 17 digits: all the digits of a double
  std::cout << "  sum of the squares:   " << ms_since(start) << " ms (sum = " << std::setprecision(17) << sum
            << std::setprecision(6) << ")" << std::endl;

  // 4. Sort a copy of x
  std::vector<double> z = x;
  start = Clock::now();
  std::sort(z.begin(), z.end());
  std::cout << "  sort:                 " << ms_since(start) << " ms" << std::endl;
}

int main() {
  // How many threads can run at the same time on this machine (inside the
  // container: how many cores Docker gives you)
  std::cout << "Threads: " << std::thread::hardware_concurrency() << std::endl;

  // 10 million random numbers between 0 and 10
  const std::size_t n = 10'000'000;
  std::mt19937 gen{42}; // random number generator, with a fixed seed
  std::uniform_real_distribution<double> random_number(0., 10.);
  std::vector<double> x(n);
  for (double &v : x)
    v = random_number(gen);

  // The first parallel algorithm also creates the threads, and this takes time.
  // We call one here, so that the creation of the threads is not in the times below
  std::vector<double> warm_up(1000);
  std::fill(std::execution::par, warm_up.begin(), warm_up.end(), 1.);

  run("seq", std::execution::seq, x);
  run("par", std::execution::par, x);
  run("par_unseq", std::execution::par_unseq, x);
  return 0;
}
