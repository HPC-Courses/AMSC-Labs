/*
Lifetime and ownership with std::unique_ptr.

compile with
  g++ -Wall -std=c++20 -g -fsanitize=address unique_ptr.cpp -o unique_ptr
and run with
  ./unique_ptr

Before running, try to predict the order of the "+" and "-" lines!
*/

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// A class that tells us when it is created and destroyed
class Tracer {
public:
  explicit Tracer(std::string name) : name_{std::move(name)} {
    std::cout << "  + Tracer(" << name_ << ")" << std::endl;
  }
  ~Tracer() { std::cout << "  - ~Tracer(" << name_ << ")" << std::endl; }
  std::string const &name() const { return name_; }

private:
  std::string name_;
};

// Observes the object: no ownership involved, a reference is enough
void print(Tracer const &t) { std::cout << "  print: " << t.name() << std::endl; }

// Sink: takes ownership. The object dies at the end of this function
void consume(std::unique_ptr<Tracer> p) {
  std::cout << "  consume: " << p->name() << std::endl;
}

// Source: creates and hands over ownership
std::unique_ptr<Tracer> make_tracer(std::string const &name) {
  auto p = std::make_unique<Tracer>(name);
  return p; // no std::move needed
}

// The same leak of raw_pointers.cpp, now impossible
void may_throw() {
  auto p = std::make_unique<Tracer>("exception-safe");
  throw std::runtime_error("something went wrong");
}

int main() {
  std::cout << "1. scope" << std::endl;
  {
    auto a = std::make_unique<Tracer>("a");
    print(*a);
  } // a destroyed here

  std::cout << "2. move" << std::endl;
  auto b = std::make_unique<Tracer>("b");
  //auto c = b;  // ERROR: copy constructor is deleted. Try it!
  auto c = std::move(b);
  std::cout << "  b is " << (b ? "not empty" : "empty") << ", c owns " << c->name() << std::endl;

  std::cout << "3. reset and assignment" << std::endl;
  c.reset(new Tracer("c2")); // the old object ("b") is destroyed
  c = std::make_unique<Tracer>("c3"); // "c2" is destroyed
  c.reset(); // "c3" is destroyed, c is empty

  std::cout << "4. source and sink" << std::endl;
  auto d = make_tracer("d");
  consume(std::move(d));
  std::cout << "  back in main, d is " << (d ? "not empty" : "empty") << std::endl;

  std::cout << "5. get and release" << std::endl;
  auto e = std::make_unique<Tracer>("e");
  Tracer *observer = e.get(); // non-owning: never delete it!
  print(*observer);
  Tracer *raw = e.release(); // e gives up ownership, now WE must delete
  delete raw;

  std::cout << "6. containers" << std::endl;
  {
    std::vector<std::unique_ptr<Tracer>> v;
    v.push_back(std::make_unique<Tracer>("v0"));
    v.emplace_back(std::make_unique<Tracer>("v1"));
    // for (auto p : v)
    for (auto const &p : v) // by reference: a copy would not compile
      print(*p);
    std::cout << "  leaving the scope of v" << std::endl;
  }

  std::cout << "7. exceptions" << std::endl;
  try {
    may_throw();
  } catch (std::exception const &ex) {
    std::cout << "  caught: " << ex.what() << std::endl;
  }

  std::cout << "end of main" << std::endl;
  return 0;
}
