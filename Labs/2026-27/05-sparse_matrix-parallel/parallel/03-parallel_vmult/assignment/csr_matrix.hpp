#ifndef CSR_MATRIX_HPP
#define CSR_MATRIX_HPP

#include "map_matrix.hpp"
#include "sparse_matrix.hpp"

#include <algorithm>
#include <execution>
#include <numeric>
#include <stdexcept>

// Sparse matrix in Compressed Sparse Row format. Three vectors:
// - values_:  the stored entries, row by row;
// - col_idx_: the column of each entry;
// - row_ptr_: the entries of row i are in the positions [row_ptr_[i], row_ptr_[i + 1]).
// For the 5x5 tridiagonal matrix:
//   values_  = { -2, 1,   1, -2, 1,   1, -2, 1,   1, -2, 1,   1, -2 }
//   col_idx_ = {  0, 1,   0,  1, 2,   1,  2, 3,   2,  3, 4,   3,  4 }
//   row_ptr_ = {  0,      2,          5,          8,          11,     13 }
template <typename T>
class CsrMatrix : public SparseMatrix<T> {
public:
  using Vector = typename SparseMatrix<T>::Vector;

  // Compresses a MapMatrix<T>. Its rows are std::maps, so the columns of each
  // row are already sorted.
  explicit CsrMatrix(const MapMatrix<T> &A) : SparseMatrix<T>{A.nrows(), A.ncols()} {
    row_ptr_.push_back(0); // row 0 starts at position 0
    for (std::size_t i = 0; i < A.nrows(); ++i) {
      for (const auto &[j, a_ij] : A.row(i)) {
        col_idx_.push_back(j);
        values_.push_back(a_ij);
      }
      row_ptr_.push_back(values_.size()); // row i ends here, row i + 1 starts here
    }

    // The indices of the rows, 0, 1, ..., nrows - 1: vmult_parallel needs them
    rows_.resize(A.nrows());
    std::iota(rows_.begin(), rows_.end(), 0);
  }

  std::size_t nnz() const override { return values_.size(); }

  // Read access: looks for column j among the entries of row i
  T operator()(std::size_t i, std::size_t j) const override {
    for (std::size_t k = row_ptr_[i]; k < row_ptr_[i + 1]; ++k)
      if (col_idx_[k] == j)
        return values_[k];
    return T{};
  }

  // y = A x, one row after the other
  Vector vmult(const Vector &x) const override {
    if (x.size() != this->ncols())
      throw std::invalid_argument("CsrMatrix::vmult: wrong size of x");
    Vector y(this->nrows());
    for (std::size_t i = 0; i < this->nrows(); ++i)
      y[i] = row_times_x(i, x);
    return y;
  }

  // y = A x, with the rows divided among several threads
  Vector vmult_parallel(const Vector &x) const {
    if (x.size() != this->ncols())
      throw std::invalid_argument("CsrMatrix::vmult_parallel: wrong size of x");
    Vector y(this->nrows());
    // TODO: the same loop as in vmult, written with std::for_each and
    //       std::execution::par. You cannot loop over i = 0, ..., nrows - 1 with
    //       an algorithm: loop over the vector rows_, which contains these numbers.
    return y;
  }

protected:
  void print_entries(std::ostream &os) const override {
    for (std::size_t i = 0; i < this->nrows(); ++i)
      for (std::size_t k = row_ptr_[i]; k < row_ptr_[i + 1]; ++k)
        os << i << ", " << col_idx_[k] << ", " << values_[k] << std::endl;
  }

private:
  // The product of row i of A with x: the entry i of y = A x
  T row_times_x(std::size_t i, const Vector &x) const {
    T sum{};
    for (std::size_t k = row_ptr_[i]; k < row_ptr_[i + 1]; ++k)
      sum += values_[k] * x[col_idx_[k]];
    return sum;
  }

  std::vector<T> values_;
  std::vector<std::size_t> col_idx_;
  std::vector<std::size_t> row_ptr_;
  std::vector<std::size_t> rows_; // 0, 1, ..., nrows - 1
};

#endif /* CSR_MATRIX_HPP */
