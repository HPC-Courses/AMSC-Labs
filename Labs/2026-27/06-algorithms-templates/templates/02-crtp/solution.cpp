/*
The shapes of Lab 3 and Lab 4, for the third time: static polymorphism with the
CRTP. Solution.

compile with
  g++ -Wall -std=c++20 solution.cpp -o solution
and run with
  ./solution
*/

#include <cmath>
#include <iostream>
#include <numbers>
#include <string>
#include <vector>

// The base class knows the type of its derived class: it is its template argument
template <typename Derived>
class Shape {
public:
  // The base delegates to the derived class: no virtual, no table of functions
  std::string name() const { return self().name(); }

  // 1.
  double area() const { return self().area(); }

  // 2. Written once, in terms of area(), for all the shapes
  double equivalent_radius() const { return std::sqrt(area() / std::numbers::pi); }

private:
  // *this is a Shape<Derived>; we know that it is the base of a Derived
  const Derived &self() const { return static_cast<const Derived &>(*this); }
};

class Circle : public Shape<Circle> {
public:
  explicit Circle(double r) : r_{r} {}
  std::string name() const { return "Circle"; }
  double area() const { return std::numbers::pi * r_ * r_; }

private:
  double r_;
};

class Square : public Shape<Square> {
public:
  explicit Square(double side) : side_{side} {}
  std::string name() const { return "Square"; }
  double area() const { return side_ * side_; }

private:
  double side_;
};

// 3.
class Rectangle : public Shape<Rectangle> {
public:
  Rectangle(double base, double height) : base_{base}, height_{height} {}
  std::string name() const { return "Rectangle"; }
  double area() const { return base_ * height_; }

private:
  double base_;
  double height_;
};

// One function for all the shapes: the compiler generates one version for each
// Derived, and knows which area() it calls
template <typename Derived>
void print(const Shape<Derived> &s) {
  std::cout << s.name() << ": area " << s.area() << ", equivalent radius " << s.equivalent_radius()
            << std::endl;
}

int main() {
  print(Circle{1.});
  print(Square{2.});
  print(Rectangle{2., 3.});
  return 0;
}
