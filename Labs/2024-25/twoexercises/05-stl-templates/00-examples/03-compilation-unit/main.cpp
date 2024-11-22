#include "sum.hpp"
// problem: multiple definitions
double sum(double a, double b) {
  return a + b;
}

int main() {
  sum(5, 4);
  return 0;
}