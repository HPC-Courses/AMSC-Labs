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


template<typename T>
class SparseMatrix {
public:
  using Vector = std::vector<T>;
  SparseMatrix() : m_nnz(0), m_nrows(0), m_ncols(0) {};
  size_t nrows() const { return m_nrows; }
  size_t ncols() const { return m_ncols; }
  size_t nnz() const { return m_nnz; }

  void print(std::ostream& os = std::cout) const {
    os << "nrows: " << m_nrows << " | ncols:" << m_ncols << " | nnz: " << m_nnz << std::endl;
    _print(os);
  };

  virtual Vector vmult(const Vector& v) const = 0;
  virtual const T& operator()(size_t i, size_t j) const = 0;
  virtual T& operator()(size_t i, size_t j) = 0;
  virtual ~SparseMatrix() = default;

protected:
  virtual void _print(std::ostream& os) const = 0;
  size_t m_nnz;
  size_t m_nrows, m_ncols;
};

template<typename T>
class MapMatrix : public SparseMatrix<T> {
public:
  using Vector = typename SparseMatrix<T>::Vector;
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

  std::vector<std::map<size_t, T>> m_data;
};

int main() {
  MapMatrix<double> m;
  m(0, 0) = 1;
  m(1, 1) = 1;
  m.print(std::cout);
  const auto x = m.vmult({{2, 3}});
  std::cout << x[0] << " " << x[1] << std::endl;
  return 0;
}