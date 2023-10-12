#include <array>
#include <vector>
#include <numbers>
#include <memory>
#include <chrono>
#include <iostream>

class Shape {
public:
    Shape(const std::string& name) : m_name(name) {};
    virtual double getArea() const = 0;
    const std::string& getName() const { return m_name; };
    // always make base classes' destructors virtual when they're meant to be manipulated polymorphically.
    virtual ~Shape() = default;
private:
    const std::string m_name;
};

class Circle : public Shape {
public:
    Circle(double radius) : Shape("Circle"), m_radius(radius) {};
    virtual double getArea() const override { return m_radius * m_radius * std::numbers::pi_v<double>; };
    virtual ~Circle() override = default;
private:
    const double m_radius;
};

class Rectangle : public Shape {
public:
    Rectangle(double b, double h) : Shape("Rectangle"), m_basis(b), m_height(h) {};
    virtual double getArea() const override { return m_basis * m_height; };
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