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

using elem_t = double;
using Vector = std::vector<elem_t>;

class SparseMatrix {
public:
  SparseMatrix() : m_nnz(0), m_nrows(0), m_ncols(0) {};
  size_t nrows() { return m_nrows; }
  size_t ncols() { return m_ncols; }
  size_t nnz() { return m_nnz; }

  void print(std::ostream& os) {
    std::cout << "nrows: " << m_nrows << " | ncols:" << m_ncols << " | nnz: " << m_nnz << std::endl;
    _print(os);
  };

  virtual Vector vmult(const Vector& v) const = 0;
  virtual const elem_t& operator()(size_t i, size_t j) const = 0;
  virtual elem_t& operator()(size_t i, size_t j) = 0;
  virtual ~SparseMatrix() = default;

protected:
  virtual void _print(std::ostream& os) const = 0;
  size_t m_nnz;
  size_t m_nrows, m_ncols;
};

class MapMatrix : public SparseMatrix {
public:
  virtual Vector vmult(const Vector& x) const override {
    // assert x.size() == m_ncols
    // allocate memory for result and initialize to 0
    // loop over each element of the matrix and add contribute to result
  }

  virtual double& operator()(size_t i, size_t j) override {
    // check if we have enough rows, if not add them
    // find column entry, if not present add it
    // return value reference
  }
  virtual const double& operator()(size_t i, size_t j) const override {
    // return value reference with no check, we use the c++ convention of no bounds check 
  }
  virtual ~MapMatrix() override = default;
protected:
  virtual void _print(std::ostream& os) const {
    // print the data
  }

  /*the data is stored in a vector of maps of type size_t, elem_t*/ m_data;
};

int main() {
  MapMatrix m;
  m(0, 0) = 1;
  m(1, 1) = 1;
  m.print(std::cout);
  const auto x = m.vmult({{2, 3}});
  std::cout << x[0] << " " << x[1] << std::endl;
  return 0;
}