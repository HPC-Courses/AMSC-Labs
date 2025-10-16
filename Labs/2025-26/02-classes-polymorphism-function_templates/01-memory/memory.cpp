/*
compile with
g++ -Wall memory.cpp -o memory

or, equivalently, with
clang++ -Wall memory.cpp -o memory

and run with
./memory
*/

/**
 * @file memory.cpp
 * @brief Demonstrates different memory allocation types and memory addressing in C++, pecifically showcasing different memory regions
 *
 * This program illustrates 
 * 1. Static memory allocation (static variables)
 * 2. Stack memory allocation (local variables)
 * 3. Heap memory allocation (dynamic memory)
 * 4. Memory address printing
 * 5. Vector memory reallocation
 */

#include <iostream>
#include <vector>
#include <memory>

/**
 * @class Counter
 * @brief A simple class that tracks its own instantiation count
 *
 * Demonstrates object construction, copy construction, and destruction
 * by incrementing/decrementing a static count
 */
struct Counter {  
  /** Constructor increments the static count */
  Counter() { ++count; }
  
  /** Copy constructor increments the static count */
  Counter(const Counter&) { ++count; }
  
  /** Destructor decrements the static count */
  ~Counter() { --count; }
  
  /** Static member to track total Counter instances */
  static size_t count;
};

// Initialize the static count
size_t Counter::count = 0;

/**
 * @brief Demonstrates memory addressing for stack and heap variables
 * 
 * This function creates
 * - A local integer variable (z) on the stack
 * - A vector of integers (v) initialized with 3 elements
 * 
 * Prints the memory addresses of
 * - The vector's data storage
 * - The local integer's address
 * 
 */
void a_function() {
  // Declare a local integer on the stack
  int z;
  // Create a vector with 3 initial elements (heap allocation)
  std::vector<int> v(3);
  // Print addresses
  std::cout << "Heap memory 2 (v after init): " << v.data() << std::endl;
  std::cout << "Stack memory (z): " << &z << std::endl;
}

/**
 * @brief Main function demonstrating various memory allocation types
 *
 * Shows memory addresses of
 * - Static variable (Counter::count)
 * - Stack variables (y, a, c)
 * - Heap variables (vector, shared_ptr)
 *
 * @return int Exit status
 */
int main() {
    // Print static memory address
    std::cout << "Static memory: " << &Counter::count << std::endl;

    // Stack memory variables
    int y;
    int a[3];
    Counter c;

    // Print stack memory addresses
    std::cout << "Stack memory (y): " << &y << std::endl;
    std::cout << "Stack memory (a): " << &a << std::endl;
    std::cout << "Stack memory (c): " << &c << std::endl;
    
    // Demonstrate memory addressing in a separate function
    // Calls a_function() to show stack and heap memory addresses
    a_function();

    // Demonstrate heap memory and reallocation
    std::vector<int> v(3);
    auto pt = std::make_shared<Counter>();
    
    // Print initial heap memory address
    std::cout << "Heap memory (v after init): " << v.data() << std::endl;
    std::cout << "Heap memory (pt): " << pt.get() << std::endl;
    
    // Cause vector reallocation
    for(int i = 0; i < 1e5; i++)
      v.push_back(0);
      
    // Print new heap memory address after reallocation
    std::cout << "Heap memory (v after push_back): " << v.data() << std::endl;
    
    return 0;
}
