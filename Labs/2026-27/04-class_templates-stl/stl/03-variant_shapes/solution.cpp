/*
The shapes of Lab 3 without inheritance: std::variant and std::visit. Solution.

compile with
  g++ -Wall -std=c++20 solution.cpp -o solution
and run with
  ./solution
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

// 4. A new type: we write the struct, and we add it to the list of the variant
struct Triangle {
  double a;
  double b;
  double c;
  double area() const { // Heron's formula
    const double s = perimeter() / 2.;
    return std::sqrt(s * (s - a) * (s - b) * (s - c));
  }
  double perimeter() const { return a + b + c; }
  std::string name() const { return "Triangle"; }
};

// A Shape holds ONE of the four types, by value: no pointer, no heap
using Shape = std::variant<Circle, Rectangle, Square, Triangle>;

int main() {
  const std::vector<Shape> shapes{Circle{1.}, Rectangle{2., 3.}, Square{1.5}, Circle{0.5},
                                  Triangle{3., 4., 6.}};

  // 1. The generic lambda is instantiated once for each type of the variant: the
  //    choice of the version to call is made at run time, by std::visit
  for (const auto &s : shapes)
    std::visit(
        [](const auto &x) {
          std::cout << "  " << x.name() << ": area " << x.area() << ", perimeter " << x.perimeter()
                    << std::endl;
        },
        s);

  // 2. std::visit returns what the lambda returns
  double total = 0.;
  for (const auto &s : shapes)
    total += std::visit([](const auto &x) { return x.area(); }, s);
  std::cout << "Total area: " << total << std::endl;

  // 3.
  int circles = 0;
  for (const auto &s : shapes)
    if (std::holds_alternative<Circle>(s))
      ++circles;
  std::cout << "Circles: " << circles << std::endl;
  return 0;
}
