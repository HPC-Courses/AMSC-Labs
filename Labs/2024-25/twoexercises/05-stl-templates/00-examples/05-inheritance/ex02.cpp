#include <iostream>
#include <vector>
#include <memory>
#include <map>

using BaseMatrix = std::vector<double *>;

class DenseMatrix : public BaseMatrix {
public:
  DenseMatrix(size_t n) : BaseMatrix(n) {
    for(auto &e : *this)
      e = new double[n];
  }
  ~DenseMatrix() { 
    std::cout << "Bye from DenseMatrix\n";
    for(auto &e : *this) {
      if(e) {
        delete[] e;
      }
    }
  }
};

class SparseMatrix : public BaseMatrix {
public:
  SparseMatrix(size_t n) : BaseMatrix(n) {
    for(auto &e : *this)
      // allocate constant memory for each row,
      // for instance for FEM matrices constant memory is enough
      e = new double[5];
  }
  ~SparseMatrix() { 
    std::cout << "Bye from SparseMatrix\n";
    for(auto &e : *this) {
      if(e) {
        delete[] e;
      }
    }
  }
};

void do_something(const std::unique_ptr<BaseMatrix> &p) {
  // call some method of p
}

int main () {
  std::unique_ptr<BaseMatrix> p = std::make_unique<SparseMatrix>(10);
  do_something(p);
  return 0;
};