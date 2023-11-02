#include <stddef.h>  // for offsetof()

#include <chrono>
#include <iostream>
#include <random>
#include <vector>

/// Layout:
/// | c | padding | padding | padding | // "i" would not fit in this row!
/// | i |    i    |    i    |    i    |
/// | s |    s    | padding | padding |
struct Struct1 {
  char c;       // 1 byte.
  int i;        // 4 bytes.
  short int s;  // 2 bytes.
};

// specify the padding
// https://carlosvin.github.io/langs/en/posts/cpp-pragma-pack/
// can give even more precise directions with gcc using
// `__attribute__((aligned))`
// https://stackoverflow.com/questions/14179748/what-is-the-difference-between-pragma-pack-and-attribute-aligned
#pragma pack(push, 1)
/// Layout:
/// | c | i | i | i |
/// | i | s | s |
struct Struct1Pack1 {
  char c;       // 1 byte.
  int i;        // 4 bytes.
  short int s;  // 2 bytes.
};
#pragma pack(pop)  // disables the effect of #pragma pack from now on

#pragma pack(push, 2)
/// Layout:
/// | c | padding | i | i |
/// | i |    i    | s | s |
struct Struct1Pack2 {
  char c;       // 1 byte.
  int i;        // 4 bytes.
  short int s;  // 2 bytes.
};
#pragma pack(pop)  // disables the effect of #pragma pack from now on

#pragma pack(push, 4)
/// Layout:
/// | c | padding | padding | padding |
/// | i |    i    |    i    |    i    |
/// | s |    s    | padding | padding |
struct Struct1Pack4 {
  char c;       // 1 byte.
  int i;        // 4 bytes.
  short int s;  // 2 bytes.
};
#pragma pack(pop)  // disables the effect of #pragma pack from now on

/// Layout:
/// | i | i | i |    i    |
/// | s | s | c | padding |
struct Struct2 {
  int i;        // 4 bytes.
  short int s;  // 2 bytes.
  char c;       // 1 byte.
};

template <class Struct>
void print_padding_info() {
  std::cout << "Size: " << sizeof(Struct) << " bytes." << std::endl;
  Struct instance;
  // static cast *char, otherwise interpreted as string
  std::cout << "c address: " << static_cast<void *>(&instance.c)
            << " | offset: " << offsetof(Struct, c) << std::endl;
  std::cout << "i address: " << &instance.i
            << " | offset: " << offsetof(Struct, i) << std::endl;
  std::cout << "s address: " << &instance.s
            << " | offset: " << offsetof(Struct, s) << std::endl;
}

template <class Struct>
void test_allocate_write_read() {
  using namespace std::chrono;

  // test allocation speed
  const auto t0 = high_resolution_clock::now();
  std::vector<Struct> elements(100'000'000);
  const auto t1 = high_resolution_clock::now();

  const auto dt = duration_cast<milliseconds>(t1 - t0).count();
  const auto start = reinterpret_cast<uintptr_t>(&elements[0]);
  const auto end = reinterpret_cast<uintptr_t>(&elements.back());
  const auto size = (end - start + 1ull) / 1024ull / 1024ull;
  std::cout << "Allocation took: " << dt << " [ms] for: " << size << " [MB]"
            << std::endl;
  const auto speed = double(size) * (1.0e3 / 1024.0) / double(dt);
  std::cout << "Allocation speed: " << speed << " [GB/s]" << std::endl;

  // random engine to test writing/reading random numbers
  std::default_random_engine engine(std::random_device{}());
  std::uniform_int_distribution<int> rand_int(1, 10);

  // test write speed
  {
    const auto t0 = high_resolution_clock::now();
    for (auto &e : elements) e.i = rand_int(engine);
    const auto t1 = high_resolution_clock::now();
    const auto dt = duration_cast<milliseconds>(t1 - t0).count();
    std::cout << "Write time: " << dt << " [ms]" << std::endl;
  }

  // test read speed
  {
    int sum = 0;
    const auto t0 = high_resolution_clock::now();
    for (auto &e : elements) sum += e.i;
    const auto t1 = high_resolution_clock::now();
    const auto dt = duration_cast<milliseconds>(t1 - t0).count();
    std::cout << "Read time: " << dt << " [ms] " << sum << std::endl;
  }
}

int main() {
  std::cout << "Struct1" << std::endl;
  print_padding_info<Struct1>();
  test_allocate_write_read<Struct1>();
  std::cout << "--------------------------------------" << std::endl;

  std::cout << "Struct1Pack1" << std::endl;
  print_padding_info<Struct1Pack1>();
  test_allocate_write_read<Struct1Pack1>();
  std::cout << "--------------------------------------" << std::endl;

  std::cout << "Struct1Pack2" << std::endl;
  print_padding_info<Struct1Pack2>();
  test_allocate_write_read<Struct1Pack2>();
  std::cout << "--------------------------------------" << std::endl;

  std::cout << "Struct1Pack4" << std::endl;
  print_padding_info<Struct1Pack4>();
  test_allocate_write_read<Struct1Pack4>();
  std::cout << "--------------------------------------" << std::endl;

  std::cout << "Struct2" << std::endl;
  print_padding_info<Struct2>();
  test_allocate_write_read<Struct2>();
  std::cout << "--------------------------------------" << std::endl;
  return 0;
}
