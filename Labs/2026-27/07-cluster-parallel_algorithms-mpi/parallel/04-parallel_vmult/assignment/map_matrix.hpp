#ifndef MAP_MATRIX_HPP
#define MAP_MATRIX_HPP

#include "sparse_matrix.hpp"

#include <map>
#include <stdexcept>

// Sparse matrix in an UNCOMPRESSED format: each row is a map {column -> value}.
// Adding an entry is cheap, so the pattern of the matrix can change at any time.
//
// Map is a template template parameter: it is not a type, but a class template,
// such as std::map or std::unordered_map, which MapMatrix instantiates as
// Map<std::size_t, T>. We write "typename..." because std::map has more than two
// template parameters (the comparison and the allocator have a default).
template <typename T, template <typename...> class Map = std::map>
class MapMatrix : public SparseMatrix<T> {
public:
  // Vector depends on T: "typename" tells the compiler that it is a type
  using Vector = typename SparseMatrix<T>::Vector;
  using Row = Map<std::size_t, T>;

  MapMatrix(std::size_t nrows, std::size_t ncols) : SparseMatrix<T>{nrows, ncols}, rows_(nrows) {}

  std::size_t nnz() const override {
    std::size_t n = 0;
    for (const auto &row : rows_)
      n += row.size();
    return n;
  }

  // Read access: it never inserts. Returns 0 if (i, j) is not stored
  T operator()(std::size_t i, std::size_t j) const override {
    const auto it = rows_[i].find(j);
    if (it == rows_[i].end())
      return T{}; // T{} is 0 for the arithmetic types
    return it->second;
  }

  // Write access: inserts the entry (i, j) if it is not stored yet
  T &operator()(std::size_t i, std::size_t j) {
    // nrows() is a member of a base class that depends on T: we need this->
    if (i >= this->nrows() || j >= this->ncols())
      throw std::out_of_range("MapMatrix: index out of range");
    return rows_[i][j];
  }

  Vector vmult(const Vector &x) const override {
    if (x.size() != this->ncols())
      throw std::invalid_argument("MapMatrix::vmult: wrong size of x");
    Vector y(this->nrows()); // all zeros
    for (std::size_t i = 0; i < this->nrows(); ++i)
      for (const auto &[j, a_ij] : rows_[i])
        y[i] += a_ij * x[j];
    return y;
  }

  // The stored entries of row i, as a map {column -> value}
  const Row &row(std::size_t i) const { return rows_[i]; }

protected:
  void print_entries(std::ostream &os) const override {
    for (std::size_t i = 0; i < this->nrows(); ++i)
      for (const auto &[j, a_ij] : rows_[i])
        os << i << ", " << j << ", " << a_ij << std::endl;
  }

private:
  std::vector<Row> rows_;
};

#endif /* MAP_MATRIX_HPP */
