#include <iostream>

class Noisy {
  public:
  Noisy() { std::cout << "constructed at " << this << '\n'; }
  Noisy(const Noisy&) { std::cout << "copy-constructed\n"; }
  Noisy(Noisy&&) { std::cout << "move-constructed\n"; }
  ~Noisy() { std::cout << "destructed at " << this << '\n'; }
};

Noisy f() {
  Noisy v = Noisy(); // copy elision when initializing v
  // from a temporary (until C++17) / prvalue (since C++17)
  return v; // NRVO from v to the result object (not guaranteed, even in C++17)
}             // if optimization is disabled, the move constructor is called

void g(Noisy arg) {
  std::cout << "&arg = " << &arg << '\n';
}

int main() {
  Noisy v = f(); // copy elision in initialization of v
  // from the temporary returned by f() (until C++17)
  // from the prvalue f() (since C++17)

  std::cout << "&v = " << &v << '\n';

  g(f()); // copy elision in initialization of the parameter of g()
  // from the temporary returned by f() (until C++17)
  // from the prvalue f() (since C++17)
}