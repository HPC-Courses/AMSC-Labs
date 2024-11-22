#include "sparse_matrix.hpp"

template <class T>
void SparseMatrix<T>::update_properties() {
}


template <class T>
std::ostream &operator<<(std::ostream &stream, SparseMatrix<T> &M) {
}

template <class T>
void
SparseMatrix<T>::aij(std::vector<T>            &a,
                     std::vector<unsigned int> &i,
                     std::vector<unsigned int> &j) {
}

template <class T>
void
SparseMatrix<T>::csr(std::vector<T>            &val,
                     std::vector<unsigned int> &col_ind,
                     std::vector<unsigned int> &row_ptr)
{
}

template <class T>
void
SparseMatrix<T>::operator+=(SparseMatrix<T> &other)
{
}

template <class T>
std::vector<T>
operator*(SparseMatrix<T> &M, const std::vector<T> &x)
{
}
