/*
SFINAE: substitution failure is not an error. Before running, predict what each
line prints.

compile with
  g++ -Wall -std=c++20 sfinae.cpp -o sfinae
and run with
  ./sfinae
*/

#include <iostream>
#include <vector>

// Two overloads. The second one exists only if T has a member type value_type
template <class T>
int f(T) {
  return 1;
}

template <class T>
int f(typename T::value_type) {
  return 2;
}

int main() {
  std::cout << f<int>(10) << std::endl;
  std::cout << f<std::vector<int>>(10) << std::endl;
  std::cout << f<std::vector<int>>(std::vector<int>{}) << std::endl;
  return 0;
}
