/*
How much does polymorphism cost? The total area of n shapes, half circles and half
squares, written in the three ways we have seen:
  virtual:      Lab 3, a std::vector of std::unique_ptr to the base class
  std::variant: Lab 4, a std::vector of variants
  CRTP:         today, one std::vector for each type

compile with
  g++ -Wall -std=c++20 -O3 benchmark.cpp -o benchmark
and run with
  ./benchmark
*/

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <memory>
#include <numbers>
#include <random>
#include <variant>
#include <vector>

// Run f a few times, and return the best time in ms
template <typename F>
double best_time(const F &f) {
  double best = 1e30;
  for (int run = 0; run < 5; ++run) {
    const auto start = std::chrono::steady_clock::now();
    f();
    const std::chrono::duration<double, std::milli> elapsed = std::chrono::steady_clock::now() - start;
    best = std::min(best, elapsed.count());
  }
  return best;
}

namespace dynamic {
class Shape {
public:
  virtual ~Shape() = default;
  virtual double area() const = 0;
};

class Circle : public Shape {
public:
  explicit Circle(double r) : r_{r} {}
  double area() const override { return std::numbers::pi * r_ * r_; }

private:
  double r_;
};

class Square : public Shape {
public:
  explicit Square(double side) : side_{side} {}
  double area() const override { return side_ * side_; }

private:
  double side_;
};
} // namespace dynamic

namespace with_variant {
struct Circle {
  double r;
  double area() const { return std::numbers::pi * r * r; }
};

struct Square {
  double side;
  double area() const { return side * side; }
};

using Shape = std::variant<Circle, Square>;
} // namespace with_variant

namespace crtp {
template <typename Derived>
class Shape {
public:
  double area() const { return static_cast<const Derived &>(*this).area(); }
};

class Circle : public Shape<Circle> {
public:
  explicit Circle(double r) : r_{r} {}
  double area() const { return std::numbers::pi * r_ * r_; }

private:
  double r_;
};

class Square : public Shape<Square> {
public:
  explicit Square(double side) : side_{side} {}
  double area() const { return side_ * side_; }

private:
  double side_;
};

template <typename Derived>
double total_area(const std::vector<Derived> &shapes) {
  double total = 0.;
  for (const Shape<Derived> &s : shapes)
    total += s.area();
  return total;
}
} // namespace crtp

int main() {
  const std::size_t n = 10'000'000;

  // The sizes of the shapes, and which ones are circles: in a random order
  std::mt19937 engine{42};
  std::uniform_real_distribution<double> size{0.5, 1.5};
  std::vector<double> sizes(n);
  std::generate(sizes.begin(), sizes.end(), [&]() { return size(engine); });
  std::vector<bool> is_circle(n);
  for (std::size_t i = 0; i < n; ++i)
    is_circle[i] = i % 2 == 0;
  std::shuffle(is_circle.begin(), is_circle.end(), engine);

  std::vector<std::unique_ptr<dynamic::Shape>> pointers;
  std::vector<with_variant::Shape> variants;
  std::vector<crtp::Circle> circles;
  std::vector<crtp::Square> squares;
  pointers.reserve(n);
  variants.reserve(n);
  circles.reserve(n);
  squares.reserve(n);
  for (std::size_t i = 0; i < n; ++i) {
    if (is_circle[i]) {
      pointers.push_back(std::make_unique<dynamic::Circle>(sizes[i]));
      variants.push_back(with_variant::Circle{sizes[i]});
      circles.emplace_back(sizes[i]);
    } else {
      pointers.push_back(std::make_unique<dynamic::Square>(sizes[i]));
      variants.push_back(with_variant::Square{sizes[i]});
      squares.emplace_back(sizes[i]);
    }
  }

  double total_virtual = 0.;
  const double t_virtual = best_time([&]() {
    total_virtual = 0.;
    for (const auto &s : pointers)
      total_virtual += s->area();
  });

  double total_variant = 0.;
  const double t_variant = best_time([&]() {
    total_variant = 0.;
    for (const auto &s : variants)
      total_variant += std::visit([](const auto &x) { return x.area(); }, s);
  });

  double total_crtp = 0.;
  const double t_crtp = best_time([&]() { total_crtp = crtp::total_area(circles) + crtp::total_area(squares); });

  std::cout << "n = " << n << std::endl;
  std::cout << "virtual:      " << t_virtual << " ms, total " << total_virtual << std::endl;
  std::cout << "std::variant: " << t_variant << " ms, total " << total_variant << std::endl;
  std::cout << "CRTP:         " << t_crtp << " ms, total " << total_crtp << std::endl;
  return 0;
}
