#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <chrono>
#include <map>
#include <unordered_map>
#include <tuple>
#include <memory>
#include <cassert>
#include <utility>
#include <functional>
#include <array>


// *use templates to make your matrix usable with different types
// *just like an std::vector can contain different elements, depending on
// *what you specified
template<typename T>
class SparseMatrix {
public:
// *constructor, takes no parameters but initializes: number of non-zero elements, number of rows and cols to zero
// *getter for number of rows
// *getter for number of cols
// *getter for number of number of non-zero

// *print function: prints the number of rows, cols and nnz; moreover calls 
//                  _print, the virtual version of print that is specialized in the children class


// *abstract virtual method vmult that implements vector multiplication
// *abstract virtual method operator()(size_t i, size_t j) that implements element access in read only (const version)
// *abstract virtual method operator()(size_t i, size_t j) that implements element access in write (returns non-const reference)
// *virtual destructor

protected: // protected because need to be accessible to children!
  // *abstract virtual method _print that prints the matrix

  // *variable to store nnz
  // *variable to store number of rows
  // *variable to store number of cols
};

int main() {
  return 0;
}