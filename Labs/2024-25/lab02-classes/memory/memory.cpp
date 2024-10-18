#include <iostream>
#include <vector>
#include <memory>

struct Counter {  
  Counter() { ++count; }
  Counter(const Counter&) { ++count; }
  ~Counter() { --count; }
  static size_t count;
};

size_t Counter::count = 0;

void a_function() {
  int z;
  std::vector<int> v(3);
  std::cout << "Heap memory 2 (v after init): " << v.data() << std::endl;
  std::cout << "Stack memory (z): " << &z << std::endl;
}

int main() {
    std::cout << "Static memory: " << &Counter::count << std::endl;
    int y;
    int a[3];
    Counter c;
    
    std::cout << "Stack memory (y): " << &y << std::endl;
    std::cout << "Stack memory (a): " << &a << std::endl;
    std::cout << "Stack memory (c): " << &c << std::endl;
    a_function();

    std::vector<int> v(3);
    auto pt = std::make_shared<Counter>();
    std::cout << "Heap memory (v after init): " << v.data() << std::endl;
    std::cout << "Heap memory (pt): " << pt.get() << std::endl;
    for(int i = 0; i < 1e5; i++)
      v.push_back(0);
    std::cout << "Heap memory (v after push_back): " << v.data() << std::endl;
    
    return 0;
}
