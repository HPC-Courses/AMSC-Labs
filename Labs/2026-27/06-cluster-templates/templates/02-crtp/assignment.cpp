/*
The shapes of Lab 3 and Lab 4, for the third time: static polymorphism with the
CRTP. Write your solution where you find TODO.

compile with
  g++ -Wall -std=c++20 assignment.cpp -o assignment
and run with
  ./assignment
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

  // TODO 1: area(), as name()
  double area() const { return 0.; }

  // TODO 2: the radius of the circle with the same area. Write it here, in terms of
  // area(): it will work for all the shapes, present and future
  double equivalent_radius() const { return 0.; }

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

// TODO 3: Rectangle(base, height), following the example of Square

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
  // print(Rectangle{2., 3.}); // uncomment in task 3
  return 0;
}
