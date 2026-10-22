#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP

#include <cstddef>
#include <iostream>
#include <vector>

// The interface of a sparse matrix with entries of type T.
// Like std::vector<T>, it is a class template: SparseMatrix<double> and
// SparseMatrix<float> are two different classes.
// Like Shape in Lab 3, it is an abstract class: each storage format derives
// from it and implements the pure virtual functions.
template <typename T>
class SparseMatrix {
public:
  using Vector = std::vector<T>;

  virtual ~SparseMatrix() = default;

  std::size_t nrows() const { return nrows_; }
  std::size_t ncols() const { return ncols_; }

  // Number of stored entries
  virtual std::size_t nnz() const = 0;

  // A(i, j), read only: returns 0 if the entry (i, j) is not stored
  virtual T operator()(std::size_t i, std::size_t j) const = 0;

  // Matrix-vector product y = A x
  virtual Vector vmult(const Vector &x) const = 0;

  // Prints the sizes, then the stored entries, one "i, j, value" per line
  void print(std::ostream &os = std::cout) const {
    os << "nrows: " << nrows_ << " | ncols: " << ncols_ << " | nnz: " << nnz() << std::endl;
    print_entries(os);
  }

protected:
  // protected: only the derived classes can build a SparseMatrix
  SparseMatrix(std::size_t nrows, std::size_t ncols) : nrows_{nrows}, ncols_{ncols} {}

  virtual void print_entries(std::ostream &os) const = 0;

private:
  std::size_t nrows_;
  std::size_t ncols_;
};

#endif /* SPARSE_MATRIX_HPP */
