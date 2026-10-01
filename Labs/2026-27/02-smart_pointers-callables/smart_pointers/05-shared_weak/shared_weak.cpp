/*
Shared ownership, reference cycles, and std::weak_ptr.

compile with
  g++ -Wall -std=c++20 -g -fsanitize=address shared_weak.cpp -o shared_weak
and run with
  ./shared_weak

The second part leaks memory on purpose: look at the destructors that are
NOT printed, and at the report of AddressSanitizer (LeakSanitizer) on Linux.
Then fix it (see the TODO).
*/

#include <iostream>
#include <memory>
#include <string>
#include <utility>

// 1. Several owners: a mesh shared by two solvers
struct Mesh {
  Mesh() { std::cout << "  + Mesh" << std::endl; }
  ~Mesh() { std::cout << "  - ~Mesh" << std::endl; }
  int n_elements = 1000;
};

class Solver {
public:
  Solver(std::string name, std::shared_ptr<Mesh const> mesh)
      : name_{std::move(name)}, mesh_{std::move(mesh)} {}
  void solve() const {
    std::cout << "  " << name_ << " on " << mesh_->n_elements << " elements" << std::endl;
  }

private:
  std::string name_;
  std::shared_ptr<Mesh const> mesh_;
};

// 2. A doubly linked list node
struct Node {
  explicit Node(std::string n) : name{std::move(n)} {
    std::cout << "  + Node(" << name << ")" << std::endl;
  }
  ~Node() { std::cout << "  - ~Node(" << name << ")" << std::endl; }

  std::string name;
  std::shared_ptr<Node> next;
  std::shared_ptr<Node> prev; // TODO: this creates a cycle. Which smart pointer should it be?
};

int main() {
  std::cout << "1. shared ownership" << std::endl;
  {
    auto mesh = std::make_shared<Mesh>();
    std::cout << "  use_count = " << mesh.use_count() << std::endl;
    Solver fluid{"fluid", mesh};
    Solver structure{"structure", mesh};
    std::cout << "  use_count = " << mesh.use_count() << std::endl;
    mesh.reset(); // main gives up its share: the Mesh is still alive!
    fluid.solve();
    structure.solve();
    std::cout << "  leaving the scope of the solvers" << std::endl;
  }

  std::cout << "2. an ownership cycle" << std::endl;
  {
    auto a = std::make_shared<Node>("a");
    auto b = std::make_shared<Node>("b");
    a->next = b;
    b->prev = a;
    std::cout << "  a.use_count = " << a.use_count()
              << ", b.use_count = " << b.use_count() << std::endl;
    std::cout << "  leaving the scope of a and b" << std::endl;
  } // are the Nodes destroyed?

  std::cout << "3. observing with weak_ptr" << std::endl;
  std::weak_ptr<Mesh> w;
  {
    auto mesh = std::make_shared<Mesh>();
    w = mesh;
    if (auto p = w.lock())
      std::cout << "  mesh alive, " << p->n_elements << " elements" << std::endl;
  }
  std::cout << "  expired: " << std::boolalpha << w.expired() << std::endl;
  if (auto p = w.lock())
    std::cout << "  this is never printed" << std::endl;

  std::cout << "end of main" << std::endl;
  return 0;
}
