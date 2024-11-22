#include "sum.hpp"

// why it works? methos defined in the body are inlined

int main() {
  Sum s;
  s.do_sum(5, 4);
  return 0;
}