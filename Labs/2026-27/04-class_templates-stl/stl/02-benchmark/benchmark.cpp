/*
How much does a container cost?
The same operations on std::vector, std::list, std::deque, std::set, std::unordered_set:
  1. sum all the elements          (O(n) for every container)
  2. look for a key                (O(n), O(log n) or O(1) on average)
  3. insert elements at the front  (O(n) or O(1))

compile and run with
  g++ -Wall -std=c++20 -O3 benchmark.cpp -o benchmark && ./benchmark
*/

#include <algorithm>
#include <chrono>
#include <deque>
#include <iostream>
#include <list>
#include <numeric>
#include <random>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

using Clock = std::chrono::steady_clock;

// Milliseconds elapsed since start
double ms_since(Clock::time_point start) {
  return std::chrono::duration<double, std::milli>(Clock::now() - start).count();
}

// Sums the elements of any container: a function template
template <typename Container>
void time_sum(const std::string &name, const Container &c) {
  const auto start = Clock::now();
  const long long sum = std::accumulate(c.begin(), c.end(), 0LL);
  const double t = ms_since(start);
  std::cout << "  " << name << ": " << t << " ms (sum = " << sum << ")" << std::endl;
}

// Looks for each key with the callable contains(key) -> bool (Lab 2)
template <typename Contains>
void time_lookup(const std::string &name, const std::vector<int> &keys, const Contains &contains) {
  const auto start = Clock::now();
  std::size_t found = 0;
  for (const int k : keys)
    if (contains(k))
      ++found;
  const double t = ms_since(start);
  std::cout << "  " << name << ": " << t * 1e6 / keys.size() << " ns per lookup (found " << found << " of "
            << keys.size() << ")" << std::endl;
}

// Inserts the numbers 0, ..., m-1 at the front of an empty container
template <typename Container>
void time_push_front(const std::string &name, int m) {
  Container c;
  const auto start = Clock::now();
  for (int i = 0; i < m; ++i)
    c.insert(c.begin(), i);
  const double t = ms_since(start);
  std::cout << "  " << name << ": " << t << " ms" << std::endl;
}

int main() {
  const int n = 1'000'000;
  std::mt19937 gen{42}; // random number generator, with a fixed seed

  // The numbers 0, 1, ..., n-1 in a random order
  std::vector<int> values(n);
  std::iota(values.begin(), values.end(), 0);
  std::shuffle(values.begin(), values.end(), gen);

  // The same numbers, sorted, in four containers
  std::vector<int> vec = values;
  std::sort(vec.begin(), vec.end());
  std::list<int> lst(values.begin(), values.end());
  lst.sort(); // it relinks the nodes: neighbours in the list are no longer neighbours in memory
  const std::set<int> st(values.begin(), values.end());
  const std::unordered_set<int> ust(values.begin(), values.end());

  std::cout << "1. sum of " << n << " elements" << std::endl;
  time_sum("vector       ", vec);
  time_sum("list         ", lst);
  time_sum("set          ", st);
  time_sum("unordered_set", ust);

  // Random keys in [0, 2n): about half of them are in the containers
  std::uniform_int_distribution<int> random_key{0, 2 * n - 1};
  std::vector<int> keys(n);
  for (auto &k : keys)
    k = random_key(gen);
  // the linear search is too slow: only the first 1000 keys
  const std::vector<int> few_keys(keys.begin(), keys.begin() + 1000);

  std::cout << "2. lookup" << std::endl;
  time_lookup("vector, std::find         ", few_keys,
              [&vec](int k) { return std::find(vec.begin(), vec.end(), k) != vec.end(); });
  time_lookup("vector, std::binary_search", keys,
              [&vec](int k) { return std::binary_search(vec.begin(), vec.end(), k); });
  time_lookup("set::contains             ", keys, [&st](int k) { return st.contains(k); });
  time_lookup("unordered_set::contains   ", keys, [&ust](int k) { return ust.contains(k); });

  const int m = 100'000;
  std::cout << "3. insert " << m << " elements at the front" << std::endl;
  time_push_front<std::vector<int>>("vector", m);
  time_push_front<std::deque<int>>("deque ", m);
  time_push_front<std::list<int>>("list  ", m);
  return 0;
}
