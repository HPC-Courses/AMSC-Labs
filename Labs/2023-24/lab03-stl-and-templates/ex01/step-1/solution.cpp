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

int main() {
  return 0;
}