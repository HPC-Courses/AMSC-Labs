#ifndef CSR_MATRIX_HPP
#define CSR_MATRIX_HPP

#include "map_matrix.hpp"
#include "sparse_matrix.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

// Sparse matrix in Compressed Sparse Row format (homework). Three vectors:
// - values_:  the stored entries, row by row, sorted by column;
// - col_idx_: the column of each entry;
// - row_ptr_: the entries of row i are in [row_ptr_[i], row_ptr_[i + 1]).
// Compared to COO, the row index is not repeated for every entry.
template <typename T>
class CsrMatrix : public SparseMatrix<T> {
public:
  using Vector = typename SparseMatrix<T>::Vector;

  template <template <typename...> class Map>
  explicit CsrMatrix(const MapMatrix<T, Map> &A)
      : SparseMatrix<T>{A.nrows(), A.ncols()}, row_ptr_(A.nrows() + 1, 0) {
    values_.reserve(A.nnz());
    col_idx_.reserve(A.nnz());
    for (std::size_t i = 0; i < A.nrows(); ++i) {
      // copy row i into a vector of pairs (column, value), sorted by column
      std::vector<std::pair<std::size_t, T>> row(A.row(i).begin(), A.row(i).end());
      std::sort(row.begin(), row.end(), [](const auto &a, const auto &b) { return a.first < b.first; });
      for (const auto &[j, a_ij] : row) {
        col_idx_.push_back(j);
        values_.push_back(a_ij);
      }
      row_ptr_[i + 1] = values_.size();
    }
  }

  std::size_t nnz() const override { return values_.size(); }

  // Binary search of column j among the columns of row i
  T operator()(std::size_t i, std::size_t j) const override {
    const auto first = col_idx_.begin() + row_ptr_[i];
    const auto last = col_idx_.begin() + row_ptr_[i + 1];
    const auto it = std::lower_bound(first, last, j);
    if (it == last || *it != j)
      return T{};
    return values_[it - col_idx_.begin()];
  }

  Vector vmult(const Vector &x) const override {
    if (x.size() != this->ncols())
      throw std::invalid_argument("CsrMatrix::vmult: wrong size of x");
    Vector y(this->nrows());
    for (std::size_t i = 0; i < this->nrows(); ++i) {
      // all the entries of row i contribute to y[i]: sum them in a local variable
      T sum{};
      for (std::size_t k = row_ptr_[i]; k < row_ptr_[i + 1]; ++k)
        sum += values_[k] * x[col_idx_[k]];
      y[i] = sum;
    }
    return y;
  }

protected:
  void print_entries(std::ostream &os) const override {
    for (std::size_t i = 0; i < this->nrows(); ++i)
      for (std::size_t k = row_ptr_[i]; k < row_ptr_[i + 1]; ++k)
        os << i << ", " << col_idx_[k] << ", " << values_[k] << std::endl;
  }

private:
  std::vector<T> values_;
  std::vector<std::size_t> col_idx_;
  std::vector<std::size_t> row_ptr_;
};

#endif /* CSR_MATRIX_HPP */
