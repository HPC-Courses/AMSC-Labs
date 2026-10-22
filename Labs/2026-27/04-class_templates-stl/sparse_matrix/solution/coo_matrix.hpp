#ifndef COO_MATRIX_HPP
#define COO_MATRIX_HPP

#include "map_matrix.hpp"
#include "sparse_matrix.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

// Sparse matrix in COOrdinate format: the list of the stored entries (i, j, value),
// sorted by row and then by column, in ONE contiguous std::vector.
// The pattern is frozen: no entry can be added. In exchange, vmult is a single
// loop on contiguous memory.
template <typename T>
class CooMatrix : public SparseMatrix<T> {
public:
  using Vector = typename SparseMatrix<T>::Vector;

  struct Entry {
    std::size_t row;
    std::size_t col;
    T value;
  };

  // Compresses a MapMatrix<T, Map>, whatever its Map.
  // It is a member function template: Map is a template parameter of this
  // constructor only, not of the class.
  template <template <typename...> class Map>
  explicit CooMatrix(const MapMatrix<T, Map> &A) : SparseMatrix<T>{A.nrows(), A.ncols()} {
    entries_.reserve(A.nnz()); // one allocation, no reallocations
    for (std::size_t i = 0; i < A.nrows(); ++i)
      for (const auto &[j, a_ij] : A.row(i))
        entries_.push_back({i, j, a_ij});
    // in a std::unordered_map the columns are not sorted
    std::sort(entries_.begin(), entries_.end(), by_position);
  }

  std::size_t nnz() const override { return entries_.size(); }

  // Binary search of (i, j) in the sorted entries: O(log nnz)
  T operator()(std::size_t i, std::size_t j) const override {
    const Entry key{i, j, T{}};
    const auto it = std::lower_bound(entries_.begin(), entries_.end(), key, by_position);
    if (it == entries_.end() || it->row != i || it->col != j)
      return T{};
    return it->value;
  }

  Vector vmult(const Vector &x) const override {
    if (x.size() != this->ncols())
      throw std::invalid_argument("CooMatrix::vmult: wrong size of x");
    Vector y(this->nrows());
    for (const auto &e : entries_)
      y[e.row] += e.value * x[e.col];
    return y;
  }

protected:
  void print_entries(std::ostream &os) const override {
    for (const auto &e : entries_)
      os << e.row << ", " << e.col << ", " << e.value << std::endl;
  }

private:
  // The order of the entries: by row, then by column.
  // std::pair compares lexicographically: first the first, then the second
  static bool by_position(const Entry &a, const Entry &b) {
    return std::pair{a.row, a.col} < std::pair{b.row, b.col};
  }

  std::vector<Entry> entries_;
};

#endif /* COO_MATRIX_HPP */
