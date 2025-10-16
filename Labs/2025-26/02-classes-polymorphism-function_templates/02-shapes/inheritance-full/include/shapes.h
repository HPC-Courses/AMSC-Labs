/**
 * @file shapes.h
 * @brief Defines an abstract base class Shape and derived classes for geometric shapes
 * @details Provides an interface for calculating areas of different geometric shapes
 */
#ifndef SHAPES_HH
#define SHAPES_HH

#include <cmath>

// Ensure M_PI is defined if not already present in standard libraries
#ifndef M_PI
  #define M_PI 4*std::atan(1.)
#endif

/**
 * @class Shape
 * @brief Abstract base class for geometric shapes
 * @details Provides a common interface for different geometric shapes
 * @note Pure virtual class that cannot be instantiated directly
 */
class Shape {
public:
    /**
     * @brief Default constructor
     */
    Shape() = default;

    /**
     * @brief Virtual destructor to ensure proper cleanup of derived classes
     * @details Allows for proper polymorphic destruction of derived class objects
     */
    virtual ~Shape() = default;

    /**
     * @brief Pure virtual method to calculate the area of the shape
     * @return double Calculated area of the shape
     */
    virtual double getArea() = 0;

    /**
     * @brief Pure virtual method to get the name of the shape
     * @return const char* Name of the shape as a string
     * @note Constexpr allows compile-time resolution of the shape name
     */
    constexpr virtual const char *getName() = 0;
};

/**
 * @class Circle
 * @brief Represents a circular geometric shape
 * @details Inherits from Shape and implements area calculation for a circle
 */
class Circle: public Shape {
private:
    /// @brief Radius of the circle
    const double radius;

public:
    /**
     * @brief Construct a new Circle object
     * @param radius Radius of the circle
     */
    Circle(const double radius);

    /**
     * @brief Virtual destructor 
     * @details Ensures proper cleanup when deleting through base class pointer
     */
    virtual ~Circle() override = default;

    /**
     * @brief Calculate the area of the circle
     * @return double Area of the circle (π * r²)
     */
    double getArea() override { return(M_PI*radius*radius); }

    /**
     * @brief Get the name of the shape
     * @return const char* Returns "Circle"
     */
    constexpr virtual const char *getName() override { return "Circle"; }
};

/**
 * @class Rectangle
 * @brief Represents a rectangular geometric shape
 * @details Inherits from Shape and implements area calculation for a rectangle
 */
class Rectangle: public Shape {
private:
    /// @brief Height of the rectangle
    const double height;
    /// @brief Base (width) of the rectangle
    const double basis;

public:
    /**
     * @brief Construct a new Rectangle object
     * @param height Height of the rectangle
     * @param basis Base (width) of the rectangle
     */
    Rectangle(const double height, const double basis);

    /**
     * @brief Virtual destructor 
     * @details Ensures proper cleanup when deleting through base class pointer
     */
    virtual ~Rectangle() override = default;

    /**
     * @brief Calculate the area of the rectangle
     * @return double Area of the rectangle (height * base)
     */
    double getArea() override { return(height*basis); }

    /**
     * @brief Get the name of the shape
     * @return const char* Returns "Rectangle"
     */
    constexpr virtual const char *getName() override { return "Rectangle"; }
};

#endif
