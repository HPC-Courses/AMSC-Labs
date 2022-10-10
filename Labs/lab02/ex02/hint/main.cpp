#include <array>
#include <vector>
#include <numbers>
#include <memory>
#include <chrono>
#include <iostream>

class Shape {
public:
    // constructor that initializes the name
    // pure virtual getter for the area
    // getter for the shape name (non virtual)
    // virtual destructor
private:
    // member with the name of the shape (const)
};

// Implement the classes "Circle" and "Rectangle"
// The constructor must be empty, in the sense the name of the shape
// should not be a user's choice 


int main() {
    std::vector<std::shared_ptr<Shape>> shapes;
    shapes.push_back(std::make_shared<Circle>(1.0));
    shapes.push_back(std::make_shared<Rectangle>(2.5, 0.2));

    for (const auto& s : shapes) {
        std::cout << "I am a " << s->getName() << ", my area is: " << s->getArea() << std::endl;
    }
    return 0;
}