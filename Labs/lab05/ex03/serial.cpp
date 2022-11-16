#include <bit>
#include <bitset>
#include <iostream>
#include <map>
#include <vector>

#include "../utils.hpp"

std::vector<char> get_primes_v1(size_t n) {
  std::vector<char> is_prime(n + 1, 1);
  is_prime[0] = is_prime[1] = 0;

  for (size_t curr = 2; curr * curr < n; curr++) {
    if (is_prime[curr]) {
      for (size_t i = curr * curr; i <= n; i += curr) {
        is_prime[i] = false;
      }
    }
  }
  return is_prime;
}

std::vector<bool> get_primes_v2(size_t n) {
  std::vector<bool> is_prime(n + 1, 1);
  is_prime[0] = is_prime[1] = 0;

  for (size_t curr = 2; curr * curr < n; curr++) {
    if (is_prime[curr]) {
      for (size_t i = curr * curr; i <= n; i += curr) {
        is_prime[i] = false;
      }
    }
  }
  return is_prime;
}

std::vector<bool> get_primes_v3(size_t n) {
  std::vector<bool> is_prime((n + 1) / 2, 1);
  for (size_t curr = 3; curr * curr < n; curr += 2) {
    if (is_prime[(curr - 3) / 2]) {
      for (size_t i = curr * curr; i <= n; i += 2 * curr) {
        is_prime[(i - 3) / 2] = false;
      }
    }
  }
  return is_prime;
}

std::vector<bool> get_primes_v4(size_t n) {
  std::vector<bool> is_prime(n + 1, 1);
  is_prime[0] = is_prime[1] = 0;

  constexpr size_t N = 32768 * 8;
  for (size_t i = 0; i < n; i += N) {
    size_t end = std::min(i + N, n);
    for (size_t curr = 3; curr * curr < end; curr += 2) {
      if (is_prime[curr]) {
        size_t first = curr * curr;
        if (first <= i) {
          first = i + (curr - (i % curr)) * ((i % curr) > 0);
        }
        for (size_t j = first; j < end; j += curr) {
          is_prime[j] = false;
        }
      }
    }
  }

  return is_prime;
}

std::vector<bool> get_primes_v5(size_t n) {
  std::vector<bool> is_prime((n + 1) / 2, 1);

  constexpr size_t N = 32768 * 8 * 2;
  for (size_t i = 0; i < n; i += N) {
    const auto end = std::min(i + N, n);
    for (size_t curr = 3; curr * curr < end; curr += 2) {
      if (is_prime[(curr - 3) / 2]) {
        size_t first = curr * curr;
        if (first <= i) {
          first = i + (curr - (i % curr)) * ((i % curr) > 0);
          first += (1 - (first % 2)) * curr;
        }
        for (size_t i = first; i < end; i += 2 * curr) {
          is_prime[(i - 3) / 2] = false;
        }
      }
    }
  }

  return is_prime;
}

int main() {
  const size_t n = 100'000'000;
  std::cout << "Elapsed: " << timeit([&]() { get_primes_v1(n); }) << " [ms] for naive" << std::endl;
  std::cout << "Elapsed: " << timeit([&]() { get_primes_v2(n); }) << " [ms] for bool" << std::endl;
  std::cout << "Elapsed: " << timeit([&]() { get_primes_v3(n); }) << " [ms] for even" << std::endl;
  std::cout << "Elapsed: " << timeit([&]() { get_primes_v4(n); }) << " [ms] for cache" << std::endl;
  std::cout << "Elapsed: " << timeit([&]() { get_primes_v5(n); }) << " [ms] for even+cache" << std::endl;

  return 0;
}