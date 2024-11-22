#include "sparse_matrix.hpp"

#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

int main(int argc, char **argv) {

  // set matrix dimensions
  const unsigned int n = 10;

  // declare sparse matrix obj
  SparseMatrix<double> A;

  // resize, acts on the number of rows
  A.resize(n);

  // populate A
  for (unsigned int i = 0; i < n; ++i)
    {
      if (i > 0)
        A[i][i - 1] = -1;

      if (i < n - 1)
        A[i][i + 1] = -1;

      A[i][i] = 4;
    }

  // return A
  std::cout << "Loop through matrix entries:" << std::endl;
  for (unsigned int i = 0; i < n; ++i)
    for (const auto &[j, val] : A[i])
      std::cout << "A[" << i << "][" << j << "] = " << val << std::endl;
  std::cout << std::endl << std::endl;

  // return A with stream operator
  std::cout << "Stream operator:" << std::endl;
  std::cout << A;
  std::cout << std::endl;

  std::vector<double>       val;
  std::vector<unsigned int> col_ind, row_ptr;

  // csr format
  A.csr(val, col_ind, row_ptr);

  std::cout << "CSR vectors:" << std::endl;
  for (auto i : val)
    std::cout << i << " ";
  std::cout << std::endl;

  for (auto i : col_ind)
    std::cout << i << " ";
  std::cout << std::endl;

  for (auto i : row_ptr)
    std::cout << i << " ";
  std::cout << std::endl << std::endl;

  std::cout << "Matrix entries from CSR:" << std::endl;
  for (unsigned int i = 0; i < n; ++i)
    for (unsigned int j = row_ptr[i]; j < row_ptr[i + 1]; ++j)
      std::cout << "A[" << i << "][" << col_ind[j] << "] = " << val[j]
                << std::endl;
  std::cout << std::endl;

  // test matrix vector product
  std::cout << "Matrix-vector product:" << std::endl;
  const std::vector<double> x(n, 1.0);
  const std::vector<double> y = A * x;
  for (const auto i : y)
    std::cout << i << " ";
  std::cout << std::endl << std::endl;
}
