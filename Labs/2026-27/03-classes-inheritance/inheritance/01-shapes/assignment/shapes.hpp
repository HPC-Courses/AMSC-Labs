#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <istream>
#include <memory>
#include <string>

// Abstract base class: it defines the interface common to all shapes.
// It cannot be instantiated, since area() and perimeter() are pure virtual.
class Shape {
public:
  // A polymorphic base class must have a virtual destructor
  virtual ~Shape() = default;

  // Pure virtual: every concrete shape MUST override them
  virtual double area() const = 0;
  virtual double perimeter() const = 0;

  // Virtual with a default implementation: derived classes MAY override it
  virtual std::string name() const { return "Shape"; }
};

// final: nobody can derive from Circle
class Circle final : public Shape {
public:
  explicit Circle(double radius);
  double area() const override;
  double perimeter() const override;
  std::string name() const override { return "Circle"; }

private:
  double radius_;
};

// TODO: Rectangle(base, height), with area(), perimeter() and name().
//       Like Circle, the constructor throws std::invalid_argument if a side is not positive.
//       Should it be final? (Hint: a Square is-a Rectangle)

// TODO: Square(side), derived from Rectangle. Which methods does it need?

// TODO (homework): Triangle(a, b, c), given the three sides. The constructor throws
//       std::invalid_argument if the sides violate the triangle inequality.
//       Area with Heron's formula: sqrt(s (s-a) (s-b) (s-c)), s = half perimeter.

// Factory: reads "<type> <parameters...>" and builds the shape.
// Returns an empty pointer if the type is unknown or the parameters are missing,
// throws std::invalid_argument if the parameters are not valid.
std::unique_ptr<Shape> make_shape(std::istream &in);

#endif /* SHAPES_HPP */
