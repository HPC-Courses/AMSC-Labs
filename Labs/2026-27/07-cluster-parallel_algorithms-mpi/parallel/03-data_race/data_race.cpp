/*
A data race: several threads change the same variable at the same time.

compile with
  g++ -Wall -std=c++20 -O3 data_race.cpp -o data_race -ltbb
and run it a few times with
  ./data_race
*/

#include <algorithm>
#include <execution>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
  const int n = 10'000'000;
  std::vector<int> numbers(n);
  std::iota(numbers.begin(), numbers.end(), 0); // 0, 1, ..., n-1

  // We count the even numbers: the right answer is n / 2
  std::cout << "Expected:            " << n / 2 << std::endl;

  // 1. A loop
  int count_loop = 0;
  for (int k : numbers)
    if (k % 2 == 0)
      ++count_loop;
  std::cout << "Loop:                " << count_loop << std::endl;

  // 2. The same loop, written with std::for_each, in parallel.
  //    The lambda changes count, a variable shared by all the threads.
  int count = 0;
  std::for_each(std::execution::par, numbers.begin(), numbers.end(), [&count](int k) {
    if (k % 2 == 0)
      ++count;
  });
  std::cout << "for_each, par:       " << count << std::endl;

  // 3. The right way: an algorithm that counts, std::count_if.
  //    Each thread counts its own part, then the library sums the partial counts.
  auto is_even = [](int k) { return k % 2 == 0; };
  auto count_if = std::count_if(std::execution::par, numbers.begin(), numbers.end(), is_even);
  std::cout << "count_if, par:       " << count_if << std::endl;

  return 0;
}
