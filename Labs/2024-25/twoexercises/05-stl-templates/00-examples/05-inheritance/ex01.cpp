#include <iostream>
#include <memory>

class Base {
public:
  ~Base() { std::cout << "Bye from base\n"; }; // Non-virtual.
};

class Derived : public Base {
public:
  ~Derived() { std::cout << "Bye from derived\n"; }
};

int main () {
  std::unique_ptr<Base> p = std::make_unique<Derived>();
  return 0;
};