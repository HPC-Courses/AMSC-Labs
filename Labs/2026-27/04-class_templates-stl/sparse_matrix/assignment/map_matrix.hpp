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

  // TODO: the number of stored entries
  std::size_t nnz() const override { return 0; }

  // TODO: read access. It must NOT insert anything: return T{} (i.e. 0) if (i, j)
  //       is not stored. Hint: maps have the method find
  T operator()(std::size_t i, std::size_t j) const override {
    (void)i, (void)j; // remove this line when you use them
    return T{};
  }

  // Write access: inserts the entry (i, j) if it is not stored yet
  // TODO: throw std::out_of_range if (i, j) is outside the matrix
  T &operator()(std::size_t i, std::size_t j) { return rows_[i][j]; }

  // TODO: y = A x. Throw std::invalid_argument if the size of x is wrong
  Vector vmult(const Vector &x) const override {
    (void)x; // remove this line when you use it
    return Vector{};
  }

  // The stored entries of row i, as a map {column -> value}
  const Row &row(std::size_t i) const { return rows_[i]; }

protected:
  // TODO: print the stored entries, one "i, j, value" per line
  void print_entries(std::ostream &os) const override {
    (void)os; // remove this line when you use it
  }

private:
  std::vector<Row> rows_;
};

#endif /* MAP_MATRIX_HPP */
