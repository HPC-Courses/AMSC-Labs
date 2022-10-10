#include <iostream>

class Noisy {
  public:
  // define constructor
  // define copy constructor
  // define destructor
};

// ============================================================================
// DO NOT TOUCH FROM HERE
// just read and interpter what is happening
// ============================================================================

Noisy f() {
  Noisy v = Noisy();
  return v;
}

void g(Noisy arg) {
  std::cout << "&arg = " << &arg << '\n';
}

int main() {
  Noisy v = f();
  std::cout << "&v = " << &v << '\n';
  g(f());
  return 0;
}