#include "shapes.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

using ShapeVector = std::vector<std::unique_ptr<Shape>>;

// s is a reference to the base class: the calls are resolved at run time
void print(const Shape &s) {
  std::cout << "  " << s.name() << ": area " << s.area() << ", perimeter " << s.perimeter() << std::endl;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <shapes file>" << std::endl;
    return 1;
  }
  std::ifstream file{argv[1]};
  if (!file) {
    std::cerr << "Cannot open " << argv[1] << std::endl;
    return 1;
  }

  ShapeVector shapes;
  std::string line;
  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#')
      continue;
    std::istringstream iss{line};
    try {
      if (auto s = make_shape(iss))
        shapes.push_back(std::move(s));
      else
        std::cerr << "Skipping unknown shape: " << line << std::endl;
    } catch (const std::invalid_argument &e) {
      std::cerr << "Skipping invalid shape: " << line << " (" << e.what() << ")" << std::endl;
    }
  }

  // The same loop works for every shape, present and future
  for (const auto &s : shapes)
    print(*s);

  double total = 0.;
  for (const auto &s : shapes)
    total += s->area();
  std::cout << "Total area: " << total << std::endl;

  double max_area = 0.;
  std::string largest;
  for (const auto &s : shapes)
    if (s->area() > max_area) {
      max_area = s->area();
      largest = s->name();
    }
  std::cout << "Largest: " << largest << std::endl;

  // Slicing: a Square copied into a Rectangle object is just a Rectangle
  Square sq{2.};
  Rectangle r = sq;
  const Rectangle &ref = sq;
  std::cout << "copy: " << r.name() << ", reference: " << ref.name() << std::endl;
  return 0;
}
