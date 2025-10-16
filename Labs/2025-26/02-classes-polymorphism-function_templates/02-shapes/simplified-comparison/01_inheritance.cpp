/**
 * @file inheritance.cpp
 * @brief Demonstrates polymorphic behavior with rectangles and squares
 * @details Shows inheritance, virtual methods, and unique_ptr usage
 */
#include <iostream>
#include <vector>
#include <memory>

/**
 * @class Rectangle
 * @brief Base class representing a rectangle with area calculation and printing
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
     * @brief Virtual destructor to ensure proper cleanup of derived classes
     */
    virtual ~Rectangle() = default;

    /**
     * @brief Calculate the area of the rectangle
     * @return double Area of the rectangle
     */
    virtual double area() const { return width * height; }

    /**
     * @brief Print the area of the rectangle
     * @details Outputs area to standard output
     */
    virtual void print() const { 
        std::cout << "Rectangle area=" << area() << std::endl; 
    }
};

/**
 * @class Square
 * @brief Derived class representing a square, inheriting from Rectangle
 * @details Overrides area calculation and print method
 */
struct Square : Rectangle {
    /// Side length of the square
    double side;

    /**
     * @brief Constructor for Square
     * @param s Side length of the square
     * @note Explicitly initializes base Rectangle with side as both width and height
     */
    explicit Square(double s) : Rectangle(s, s), side(s) {}

    /**
     * @brief Calculate the area of the square
     * @return double Area of the square
     */
    double area() const override { return side * side; }

    /**
     * @brief Print the area of the square
     * @details Outputs area to standard output
     */
    void print() const override { 
        std::cout << "Square area=" << area() << std::endl; 
    }
};

/**
 * @brief Utility function to show a rectangle's properties
 * @param s Unique pointer to a Rectangle
 */
void show(const std::unique_ptr<Rectangle>& s) {
    s->print();
}

/**
 * @brief Main function demonstrating polymorphic vector of rectangles
 * @return int Exit status
 */
int main() {
    // Create a vector of unique pointers to Rectangles
    std::vector<std::unique_ptr<Rectangle>> v;
    
    // Add a Square and a Rectangle to the vector
    v.push_back(std::make_unique<Square>(4.0));
    v.push_back(std::make_unique<Rectangle>(2.0, 5.0));
    
    // Print the area of each shape
    for (const auto& r : v) {
        show(r);
    }

    return 0;
}
