/**
 * @file concepts.cpp
 * @brief Demonstrates compile-time polymorphism using C++20 concepts
 * @details Shows how concepts provide compile-time interface constraints
 * @requires C++20 compiler
 */
#include <iostream>
#include <concepts>

/**
 * @concept Shape
 * @brief Defines the requirements for a shape-like type
 * @details Enforces compile-time checking of shape interface
 * @tparam T Type to be checked against shape requirements
 */
template <typename T>
concept Shape = requires(const T& t) {
    // Concept requires:
    // 1. area() method that returns something convertible to double
    { t.area() } -> std::convertible_to<double>;
    
    // 2. print() method that returns void
    { t.print() } -> std::same_as<void>;
};

/**
 * @class Square
 * @brief Represents a square with area calculation and printing
 * @details Provides methods to calculate and display square area
 */
struct Square {
    /// Side length of the square
    double side;

    /**
     * @brief Constructor for Square
     * @param s Side length of the square
     */
    explicit Square(double s) : side(s) {}

    /**
     * @brief Calculate the area of the square
     * @return double Area of the square
     */
    double area() const { return side * side; }

    /**
     * @brief Print the area of the square
     * @details Outputs area to standard output
     */
    void print() const { 
        std::cout << "Square area=" << area() << '\n'; 
    }
};

/**
 * @class Rectangle
 * @brief Represents a rectangle with area calculation and printing
 * @details Provides methods to calculate and display rectangle area
 */
struct Rectangle {
    /// Width of the rectangle
    double w;
    /// Height of the rectangle
    double h;

    /**
     * @brief Constructor for Rectangle
     * @param ww Width of the rectangle
     * @param hh Height of the rectangle
     */
    Rectangle(double ww, double hh) : w(ww), h(hh) {}

    /**
     * @brief Calculate the area of the rectangle
     * @return double Area of the rectangle
     */
    double area() const { return w * h; }

    /**
     * @brief Print the area of the rectangle
     * @details Outputs area to standard output
     */
    void print() const { 
        std::cout << "Rectangle area=" << area() << '\n'; 
    }
};

/**
 * @brief Generic function for processing shapes using concepts
 * @tparam T Type of shape (constrained by Shape concept)
 * @param s Shape object to process
 * @details Demonstrates compile-time polymorphic behavior with concept constraints
 */
void show(const Shape auto& s) {
    // Print shape details
    s.print();
}

/**
 * @brief Demonstrate a custom type that doesn't meet Shape concept
 * @details Shows how concept constraints prevent incompatible types
 */
struct InvalidShape {
    // Intentionally missing area() or print() method
    void dummy() {}
};

/**
 * @brief Main function demonstrating concept-based polymorphism
 * @return int Exit status
 */
int main() {
    // Create shape instances
    Square sq(4.0);
    Rectangle r(2.0, 5.0);

    // Demonstrate concept-constrained polymorphism
    show(sq);  // Works fine
    show(r);   // Works fine

    // Uncommenting the line below would cause a compile-time error
    // show(InvalidShape{}); // Concept check fails

    return 0;
}
