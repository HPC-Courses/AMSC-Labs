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
    // TODO: copy the entries of A, row by row, into entries_ (A.row(i) gives you
    //       the map of row i). Then sort entries_ with by_position: why is it needed?
  }

  // TODO: the number of stored entries
  std::size_t nnz() const override { return 0; }

  // TODO: read access. entries_ is sorted: find (i, j) with a binary search,
  //       std::lower_bound with by_position. Return T{} if (i, j) is not stored
  T operator()(std::size_t i, std::size_t j) const override {
    (void)i, (void)j; // remove this line when you use them
    return T{};
  }

  // TODO: y = A x, with a single loop on entries_
  Vector vmult(const Vector &x) const override {
    (void)x; // remove this line when you use it
    return Vector{};
  }

protected:
  // TODO: print the stored entries, one "i, j, value" per line
  void print_entries(std::ostream &os) const override {
    (void)os; // remove this line when you use it
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
