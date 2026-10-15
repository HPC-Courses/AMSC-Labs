#include "shapes.hpp"

#include <numbers>
#include <stdexcept>

Circle::Circle(double radius) : radius_{radius} {
  if (radius_ <= 0.)
    throw std::invalid_argument("the radius must be positive");
}

double Circle::area() const { return std::numbers::pi * radius_ * radius_; }

double Circle::perimeter() const { return 2. * std::numbers::pi * radius_; }

std::unique_ptr<Shape> make_shape(std::istream &in) {
  std::string type;
  if (!(in >> type))
    return nullptr;

  if (type == "circle") {
    double r;
    if (in >> r)
      return std::make_unique<Circle>(r);
  }
  // TODO: "rectangle b h" and "square l" (and "triangle a b c" at home)
  return nullptr;
}
