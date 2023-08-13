#include <random>
#include <chrono>
#include <iostream>
#include <vector>

void toupper_standard(char* d, int len) {
  for (int i = 0; i < len; ++i) {
    if (d[i] >= 'a' && d[i] <= 'z')
      d[i] -= 32;
  }
}

void toupper_branchless(char* d, int len) {
  for (int i = 0; i < len; ++i) {
    d[i] -= 32 * (d[i] >= 'a' && d[i] <= 'z');
  }
}

static constexpr char alphanum[] =
"0123456789"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";

int main() {
  std::random_device r;
  std::mt19937 engine(r());

  const unsigned int num_test = 100000;
  std::uniform_int_distribution<> len_distrib(5, 50);
  std::uniform_int_distribution<> chr_distrib(0, 62); // all alphanum ascii characters 

  // build tests
  std::vector<std::vector<char>> tests;
  tests.reserve(num_test);
  for (unsigned int i = 0; i < num_test; ++i) {
    std::vector<char> tmp;
    const auto len = len_distrib(engine);
    tmp.reserve(len);
    for (int i = 0; i < len; ++i) {
      tmp.push_back(alphanum[chr_distrib(engine)]);
    }
    tests.push_back(tmp);
  }

  // run tests
  for (const auto& func : { toupper_branchless, toupper_standard }) {
    using namespace std::chrono;
    const auto t0 = high_resolution_clock::now();
    for (auto t : tests)
      func(t.data(), t.size());
    const auto t1 = high_resolution_clock::now();
    const auto dt = duration_cast<microseconds>(t1 - t0).count();
    std::cout << "Elapsed: " << dt << " [μs]" << std::endl;
  }

  return 0;
}