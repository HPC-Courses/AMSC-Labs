#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP

#include <iostream>
#include <map>
#include <vector>

template <class T>
using RowType = std::map<unsigned int, T>;

/// Templated class for sparse row-oriented matrix.
template <class T>
class SparseMatrix {
public:
  /// Default constructor.
  SparseMatrix()
    : m_nnz(0)
    , m_m(0)
  {}

  /// Implement usefull methods exploiting std::vector
  inline size_t size() const {
    return m_data.size();
  } 

  inline void resize(size_t n) {
    m_data.resize(n);
  }

  // operator[] in const and not const version, 
  // C++ chooses the best overload
  inline const RowType<T> &operator[](size_t n) const {
    return m_data[n];
  }
  
  inline RowType<T> &operator[](size_t n) {
    return m_data[n];
  }

  /// Recompute sparse matrix properties.
  void update_properties();

  /// Sparse matrix sum. Automatically allocates additional entries.
  void operator+=(SparseMatrix<T> &other);

  /// Stream operator.
  template <class U>
  friend std::ostream &
  operator<<(std::ostream &stream, SparseMatrix<U> &M);

  /// Compute matrix-vector product.
  template <class U>
  friend std::vector<U>
  operator*(SparseMatrix<U> &M, const std::vector<U> &x);

  /// Convert row-oriented sparse matrix to AIJ format.
  void
  aij(std::vector<T>            &a,
      std::vector<unsigned int> &i,
      std::vector<unsigned int> &j);

  /// Convert row-oriented sparse matrix to CSR format.
  void
  csr(std::vector<T>            &val,
      std::vector<unsigned int> &col_ind,
      std::vector<unsigned int> &row_ptr);

private:
  std::vector<RowType<T>> m_data;
  size_t m_nnz; ///< Number of nonzero elements.
  size_t m_m;   ///< Number of nonempty columns.
};

#endif /* SPARSE_MATRIX_HPP */
