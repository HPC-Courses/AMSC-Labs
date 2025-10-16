/**
 * @file function-templates.cpp
 * @brief Demonstrates compile-time polymorphism using templates
 * @details Shows how templates can provide static polymorphic behavior
 */
#include <iostream>

/**
 * @class Rectangle
 * @brief Represents a rectangle with area calculation and printing
 * @details Provides methods to calculate and display rectangle area
 */
struct Rectangle {
    /// Width of the rectangle
    double width;
    /// Height of the rectangle
    double height;

    /**
     * @brief Constructor for Rectangle
     * @param w Width of the rectangle
     * @param h Height of the rectangle
     */
    Rectangle(double w, double h) : width(w), height(h) {}

    /**
     * @brief Calculate the area of the rectangle
     * @return double Area of the rectangle
     */
    double area() const { return width * height; }

    /**
     * @brief Print the area of the rectangle
     * @details Outputs area to standard output
     */
    void print() const { 
        std::cout << "Rectangle area=" << area() << '\n'; 
    }
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
 * @brief Generic function using compile-time polymorphism
 * @tparam Shape Type of shape (deduced automatically)
 * @param s Shape object to process
 * @details Demonstrates static polymorphism via templates
 */
template <typename Shape>
void show(const Shape& s) {
    // Call print method
    s.print();
}

/**
 * @brief Main function demonstrating template-based polymorphism
 * @return int Exit status
 */
int main() {
    // Create shape instances
    Square sq(4.0);
    Rectangle r(2.0, 5.0);

    // Demonstrate compile-time polymorphic behavior
    show(sq);    // Calls Square's print method
    show(r);  // Calls Rectangle's print method

    return 0;
}
