#ifndef UTILS_HH
#define UTILS_HH


#include <iostream>
#include <cmath>
using namespace std;

#ifndef M_PI
#define M_PI 4*std::atan(1.)
#endif


// in the .h file you declare the functions, classes, ..., while in the .cpp you define them

class Shape{
    public:
    Shape() = default;
    virtual ~Shape() = default; //Always make virtual the destructor when working with class inheritance 
    virtual double getArea() = 0; //Pure virtual class
    constexpr virtual const char *getName() = 0; //Make also this virtual to resolve at compile time the name
};

class Circle: public Shape{
    private:
    const double radius;

    public:
    Circle(const double);
    virtual ~Circle() override = default;
    double getArea() override { return(M_PI*radius*radius); };
    constexpr virtual const char *getName() override { return "Circle"; };
};

class Rectangle: public Shape{
    private:
    const double height;
    const double basis;

    public:
    Rectangle(const double, const double);
    virtual ~Rectangle() override = default;
    double getArea() override { return(height*basis); };
    constexpr virtual const char *getName() override { return "Rectangle"; };
};

#endif
