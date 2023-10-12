#include <array>
#include <vector>
#include <numbers>
#include <memory>
#include <chrono>
#include <iostream>

class Shape {
public:
    Shape() = default;
    virtual double getArea() const = 0;
    // the advantage of this version w.r.t. the other is that we exploit
    // static memory, in this way classes consume less memory and the name
    // is 'embedded' at compile time into the class.
    // Here we *cannot* use std::string since it has dynamic memory
    constexpr virtual const char *getName() = 0;
    // always make base classes' destructors virtual when they're meant to be manipulated polymorphically.
    virtual ~Shape() = default;
};

class Circle : public Shape {
public:
    Circle(double radius) : Shape(), m_radius(radius) {};
    virtual double getArea() const override { return m_radius * m_radius * std::numbers::pi_v<double>; };
    constexpr virtual const char *getName() override { return "Circle"; };
    virtual ~Circle() override = default;
private:
    const double m_radius;
};

class Rectangle : public Shape {
public:
    Rectangle(double b, double h) : Shape(), m_basis(b), m_height(h) {};
    virtual double getArea() const override { return m_basis * m_height; };
    constexpr virtual const char *getName() override { return "Rectangle"; };
    virtual ~Rectangle() override = default;
private:
    const double m_basis, m_height;
};

int main() {
    std::vector<std::shared_ptr<Shape>> shapes;
    shapes.push_back(std::make_shared<Circle>(1.0));
    shapes.push_back(std::make_shared<Rectangle>(2.5, 0.2));

    // notice the use of the auto range
    for (const auto& s : shapes) {
        std::cout << "I am a " << s->getName() << ", my area is: " << s->getArea() << std::endl;
    }
    return 0;
}