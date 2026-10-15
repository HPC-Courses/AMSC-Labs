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

// Not final: Square derives from it
class Rectangle : public Shape {
public:
  Rectangle(double base, double height);
  double area() const override { return base_ * height_; }
  double perimeter() const override { return 2. * (base_ + height_); }
  std::string name() const override { return "Rectangle"; }

private:
  double base_;
  double height_;
};

// A Square is-a Rectangle: it only needs its own constructor and name()
class Square final : public Rectangle {
public:
  explicit Square(double side) : Rectangle{side, side} {}
  std::string name() const override { return "Square"; }
};

class Triangle final : public Shape {
public:
  // Throws std::invalid_argument if the sides violate the triangle inequality
  Triangle(double a, double b, double c);
  double area() const override; // Heron's formula
  double perimeter() const override { return a_ + b_ + c_; }
  std::string name() const override { return "Triangle"; }

private:
  double a_, b_, c_;
};

// Factory: reads "<type> <parameters...>" and builds the shape.
// Returns an empty pointer if the type is unknown or the parameters are missing,
// throws std::invalid_argument if the parameters are not valid.
std::unique_ptr<Shape> make_shape(std::istream &in);

#endif /* SHAPES_HPP */
