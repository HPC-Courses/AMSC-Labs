#include "sum.hpp"
// problem: no header guard
// solvable also with inline, however it is not
// a good practice to use inline if the function is long
// moreover we are not exploiting the source file
int main() {
  sum(3, 3);
  return 0;
}