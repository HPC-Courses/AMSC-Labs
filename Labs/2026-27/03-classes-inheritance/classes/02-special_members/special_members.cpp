/*
Which special member function is called, and when?

compile with
  g++ -Wall -std=c++20 special_members.cpp -o special_members
and run with
  ./special_members

Before running, write down for each numbered case which of the six special
member functions you expect to be called. Then compare.
*/

#include <iostream>
#include <string>
#include <utility>
#include <vector>

// A class that prints every special member function
class Noisy {
public:
  Noisy() { std::cout << "    default constructor" << std::endl; }
  explicit Noisy(const std::string &name) : name_{name} {
    std::cout << "    constructor(" << name_ << ")" << std::endl;
  }
  Noisy(const Noisy &other) : name_{other.name_} {
    std::cout << "    copy constructor(" << name_ << ")" << std::endl;
  }
  // TODO (case 7): remove noexcept and run again. What changes?
  Noisy(Noisy &&other) noexcept : name_{std::move(other.name_)} {
    std::cout << "    move constructor(" << name_ << ")" << std::endl;
  }
  Noisy &operator=(const Noisy &other) {
    name_ = other.name_;
    std::cout << "    copy assignment(" << name_ << ")" << std::endl;
    return *this;
  }
  Noisy &operator=(Noisy &&other) noexcept {
    name_ = std::move(other.name_);
    std::cout << "    move assignment(" << name_ << ")" << std::endl;
    return *this;
  }
  ~Noisy() { std::cout << "    destructor(" << name_ << ")" << std::endl; }

private:
  std::string name_{"unnamed"};
};

Noisy make_noisy(const std::string &name) { return Noisy{name}; }

void by_value(Noisy n) {}
void by_reference(const Noisy &) {}

int main() {
  std::cout << "1. Noisy a{\"a\"};" << std::endl;
  Noisy a{"a"};

  std::cout << "2. Noisy b = a;" << std::endl;
  // It is equivalent to Noisy b(a);
  Noisy b = a;

  std::cout << "3. b = a;" << std::endl;
  b = a;

  std::cout << "4. Noisy c = std::move(a);" << std::endl;
  Noisy c = std::move(a);

  std::cout << "5. Noisy d = make_noisy(\"d\");" << std::endl;
  Noisy d = make_noisy("d");

  std::cout << "6a. by_value(c);" << std::endl;
  by_value(c);
  std::cout << "6b. by_reference(c);" << std::endl;
  by_reference(c);

  std::cout << "7. push_back into a vector (watch the reallocation)" << std::endl;
  std::vector<Noisy> v;
  v.push_back(Noisy{"v0"});
  std::cout << "   -- second push_back" << std::endl;
  v.push_back(Noisy{"v1"});

  std::cout << "8. end of main" << std::endl;
  return 0;
}
