#include "shapes.hpp"

#include <cmath>
#include <numbers>
#include <stdexcept>

Circle::Circle(double radius) : radius_{radius} {
  if (radius_ <= 0.)
    throw std::invalid_argument("the radius must be positive");
}

double Circle::area() const { return std::numbers::pi * radius_ * radius_; }

double Circle::perimeter() const { return 2. * std::numbers::pi * radius_; }

Rectangle::Rectangle(double base, double height) : base_{base}, height_{height} {
  if (base_ <= 0. || height_ <= 0.)
    throw std::invalid_argument("the sides must be positive");
}

Triangle::Triangle(double a, double b, double c) : a_{a}, b_{b}, c_{c} {
  if (a_ <= 0. || b_ <= 0. || c_ <= 0. || a_ + b_ <= c_ || a_ + c_ <= b_ || b_ + c_ <= a_)
    throw std::invalid_argument("the sides do not form a triangle");
}

double Triangle::area() const {
  double const s = 0.5 * perimeter();
  return std::sqrt(s * (s - a_) * (s - b_) * (s - c_));
}

std::unique_ptr<Shape> make_shape(std::istream &in) {
  std::string type;
  if (!(in >> type))
    return nullptr;

  if (type == "circle") {
    double r;
    if (in >> r)
      return std::make_unique<Circle>(r);
  } else if (type == "rectangle") {
    double b, h;
    if (in >> b >> h)
      return std::make_unique<Rectangle>(b, h);
  } else if (type == "square") {
    double l;
    if (in >> l)
      return std::make_unique<Square>(l);
  } else if (type == "triangle") {
    double a, b, c;
    if (in >> a >> b >> c)
      return std::make_unique<Triangle>(a, b, c);
  }
  return nullptr;
}
