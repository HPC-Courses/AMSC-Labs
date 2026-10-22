/*
Where do the definitions of a class template go?

compile with
  g++ -Wall -std=c++20 main.cpp point.cpp -o main
*/

#include "point.hpp"

#include <iostream>

int main() {
  Point<double, 2> p{{3., 4.}};
  std::cout << "|p| = " << p.norm() << std::endl;
  return 0;
}
