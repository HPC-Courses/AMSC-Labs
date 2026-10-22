/*
The shapes of Lab 3 without inheritance: std::variant and std::visit.

compile with
  g++ -Wall -std=c++20 assignment.cpp -o assignment
and run with
  ./assignment
*/

#include <cmath>
#include <iostream>
#include <numbers>
#include <string>
#include <variant>
#include <vector>

// Plain structs: no base class, no virtual functions
struct Circle {
  double r;
  double area() const { return std::numbers::pi * r * r; }
  double perimeter() const { return 2. * std::numbers::pi * r; }
  std::string name() const { return "Circle"; }
};

struct Rectangle {
  double base;
  double height;
  double area() const { return base * height; }
  double perimeter() const { return 2. * (base + height); }
  std::string name() const { return "Rectangle"; }
};

struct Square {
  double side;
  double area() const { return side * side; }
  double perimeter() const { return 4. * side; }
  std::string name() const { return "Square"; }
};

// A Shape holds ONE of the three types, by value: no pointer, no heap
using Shape = std::variant<Circle, Rectangle, Square>;

int main() {
  const std::vector<Shape> shapes{Circle{1.}, Rectangle{2., 3.}, Square{1.5}, Circle{0.5}};

  // 1. TODO: print name, area and perimeter of each shape, as print() in Lab 3.
  //    std::visit(f, s) calls f with the type that s holds now: use a generic
  //    lambda, [](const auto &x) { ... }

  // 2. TODO: compute and print the total area

  // 3. TODO: count the circles, with std::holds_alternative<Circle>

  // 4. TODO: add a struct Triangle(a, b, c), with the area given by Heron's formula,
  //    and put a Triangle{3., 4., 6.} in shapes. What did you have to change?
  //    What would you have to change to add a method, e.g. a diameter?

  return 0;
}
