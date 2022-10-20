#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <chrono>
#include <map>
#include <unordered_map>
#include <tuple>
#include <memory>
#include <cassert>
#include <utility>
#include <functional>
#include <array>

// define generic types for the matrix in c++ style
// this is a easy way to change the type without using templates
using elem_t = double;
using Vector = std::vector<elem_t>;

// sparse abstract base class
class SparseMatrix {
public:
  SparseMatrix() : m_nnz(0), m_nrows(0), m_ncols(0) {};
  size_t nrows() const { return m_nrows; }
  size_t ncols() const { return m_ncols; }
  size_t nnz() const { return m_nnz; }

  // we should always print the dimension of the matrix
  // to avoid repeating code in the children we implement here 
  // the common part, then we delegate the rest to a protected member
  void print(std::ostream& os) const {
    os << "nrows: " << m_nrows << " | ncols:" << m_ncols << " | nnz: " << m_nnz << std::endl;
    _print(os);
  };

  virtual Vector vmult(const Vector& v) const = 0;
  // the two version for the access operator
  // a const version for read and a non-const for write
  virtual const elem_t& operator()(size_t i, size_t j) const = 0;
  virtual elem_t& operator()(size_t i, size_t j) = 0;
  // remember to use the virtual destructor!
  virtual ~SparseMatrix() = default;

protected:
  virtual void _print(std::ostream& os) const = 0;
  // protected memembers (need to be accessible to the derived class)
  // store the size of the matrix and the number of non-zero elements
  size_t m_nnz;
  size_t m_nrows, m_ncols;
};

// in a more naive version the vectors with the indices of the COO matrix are left not-sorted
// however in this way finding an element has complexity O(nnz)
// by keeping the entries sorted first by row index and then by column index, 
// we improve random access times, namely has complexity O(log nnz).
// We switch between the two versions using the KeepSorted template argument
// in this way we have no overhead at runtime 
template<bool KeepSorted>
class CooMatrix : public SparseMatrix {
public:
  virtual Vector vmult(const Vector& x) const override {
    assert(x.size() == m_ncols);
    Vector res(x.size());
    for (const auto& ijv : m_data) {
      res[std::get<0>(ijv)] += x[std::get<1>(ijv)] * std::get<2>(ijv);
    }
    return res;
  }
  virtual double& operator()(size_t i, size_t j) override {
    // look for the element with indices ij in the matrix
    const auto it = find_elem(i, j);
    const size_t idx = it - m_data.begin();
    // the element is not present, we add it
    if (it == m_data.cend()) {
      m_data.emplace_back(std::forward_as_tuple(i, j, 0));
      m_nnz++;
      m_ncols = std::max(m_ncols, j + 1);
      m_nrows = std::max(m_nrows, i + 1);
      return std::get<2>(m_data.back());
    }
    if constexpr (KeepSorted) {
      // if we keep the Vector sorted 'it' is the lower bound on the element
      // meaning that 'it' could point to an element inside the Vector that 
      // is not the element we are looking for, in this latter case we add it
      // by shifting all the elements
      if ((std::get<0>(*it) != i) || (std::get<1>(*it) != j)) {
        m_data.push_back(m_data.back());
        for (size_t k = m_data.size() - 2; k > idx; --k)
          m_data[k] = m_data[k - 1];
        m_data[idx] = std::make_tuple(i, j, 0);
        m_nnz++;
        m_ncols = std::max(m_ncols, j + 1);
        m_nrows = std::max(m_nrows, i + 1);
      }
    }
    // now the element is fore sure present, we return it
    return std::get<2>(m_data[idx]);
  }
  virtual const double& operator()(size_t i, size_t j) const override {
    const auto it = find_elem(i, j);
    return std::get<2>(*it);
  }

  virtual ~CooMatrix() override = default;
private:
  virtual void _print(std::ostream& os) const {
    for (const auto& ijv : m_data)
      os << std::get<0>(ijv) << "," << std::get<1>(ijv) << "," << std::get<2>(ijv) << std::endl;
  }
  using ijv_t = std::tuple<size_t, size_t, elem_t>;
  // utility to find element among the data
  // if we keep the Vector sorted we can find the element in O(log nnz) with std::lower_bound
  // instead of O(nnz) when using std::find_if
  std::vector<ijv_t>::const_iterator find_elem(size_t i, size_t j) const {
    // with consexpr the 'if' is resolved at complile time, we have no overhead at compile time
    if constexpr (KeepSorted) {
      return std::lower_bound(
        m_data.begin(),
        m_data.end(),
        std::make_pair(i, j),
        [](const ijv_t& x, const auto& value) {
          return (std::get<0>(x) < value.first) || ((std::get<0>(x) == value.first) && (std::get<1>(x) < value.second));
        });
    }
    else {
      return std::find_if(
        m_data.begin(),
        m_data.end(),
        [=](const auto& x) {return (std::get<0>(x) == i) && (std::get<1>(x) == j);}
      );
    }
  }

  std::vector<ijv_t> m_data;
};


// we represent each row of the sparse matrix with a map, however there are different
// possible implementations of map, for this reason we leave the choice open by using 
// a template
template<template <typename...> class MapType>
class MapMatrix : public SparseMatrix {
public:
  virtual Vector vmult(const Vector& x) const override {
    assert(x.size() == m_ncols);
    Vector res(x.size());
    for (size_t i = 0; i < m_data.size(); ++i) {
      // notice the c++17 style for range over map pairs with 
      // key 'j' and value 'v
      for (const auto& [j, v] : m_data[i]) {
        res[i] += x[j] * v;
      }
    }
    return res;
  }

  virtual double& operator()(size_t i, size_t j) override {
    // we do not have enough rows, we insert the rows needed
    if (m_data.size() < i + 1) {
      m_data.resize(i + 1);
      m_nrows = i + 1;
    }
    // look in the row for the column 'j'
    const auto it = m_data[i].find(j);
    // the column was not found, add new element with default value 0
    if (it == m_data[i].end()) {
      m_ncols = std::max(m_ncols, j + 1);
      m_nnz++;
      return (*m_data[i].emplace(j, 0).first).second;
    }
    return (*it).second;
  }
  virtual const double& operator()(size_t i, size_t j) const override {
    return m_data[i].at(j);
  }
  virtual ~MapMatrix() override = default;
protected:
  virtual void _print(std::ostream& os) const {
    for (size_t i = 0; i < m_data.size(); ++i) {
      for (const auto& [j, v] : m_data[i])
        os << i << "," << j << "," << v << std::endl;
    }
  }

private:
  std::vector<MapType<size_t, elem_t>> m_data;
};


// here we define some utility functions

// fill a tridiagonal matrix
bool fill_matrix(SparseMatrix& mm, size_t N) {
  mm(N - 1, N - 2) = 1;
  mm(N - 1, N - 1) = -2;
  for (size_t i = N - 2; i > 0; --i) {
    mm(i, i - 1) = 1;
    mm(i, i) = -2;
    mm(i, i + 1) = 1;
  }
  mm(0, 0) = -2;
  mm(0, 1) = 1;
  // this is not stictly needed, is just to check that if we assing a value to 
  // an element two times nothing bad happend 
  mm(0, 0) = -2;
  mm(0, 1) = 1;
  for (size_t i = 1; i < N - 1; ++i) {
    mm(i, i - 1) = 1;
    mm(i, i) = -2;
    mm(i, i + 1) = 1;
  }
  mm(N - 1, N - 2) = 1;
  mm(N - 1, N - 1) = -2;
  // check everything is ok
  bool ok = (mm(N - 1, N - 2) == 1) && (mm(N - 1, N - 1) == -2)
    && (mm(0, 1) == 1) && (mm(0, 0) == -2);
  if (!ok)
    return false;
  for (size_t i = N - 2; i > 0; --i) {
    if ((mm(i, i - 1) != 1) || (mm(i, i) != -2) || (mm(i, i + 1) != 1))
      return false;
  }
  return true;
}

// utility for printing the result of a test
void print_test_result(bool r, const std::string& test_name) {
  std::cout << test_name << " test: " << (r ? "PASSED" : "FAILED") << std::endl;
}

// print an iterable container
template<typename T>
void print(const T& container) {
  for (const auto& elem : container)
    std::cout << elem << ", ";
  std::cout << std::endl;
}

// check two vectors are the same
template<typename T>
bool eq(const std::vector<T>& lhs, const std::vector<T>& rhs) {
  if (lhs.size() != rhs.size())
    return false;
  for (size_t i = 0; i < lhs.size(); ++i) {
    if (lhs[i] != rhs[i])
      return false;
  }
  return true;
}

// time a function execution time
auto timeit(const std::function<void()>& f) {
  using namespace std::chrono;
  const auto t0 = high_resolution_clock::now();
  f();
  const auto t1 = high_resolution_clock::now();
  return duration_cast<microseconds>(t1 - t0).count();
}

int main() {
  constexpr size_t N = 20000; // size of the matrix
  Vector x(N), res(N); // 'res' is the vmult result of matrix * x
  std::iota(x.begin(), x.end(), 0);
  res[0] = 1;
  res[N - 1] = -elem_t(N);

  std::vector<std::shared_ptr<SparseMatrix>> matrices = {
    std::make_shared<MapMatrix<std::map>>(),
    std::make_shared<MapMatrix<std::unordered_map>>(),
    std::make_shared<CooMatrix<true>>(),
    std::make_shared<CooMatrix<false>>(),
  };

  // loop over every matrix and test and benckmark it
  for (auto& mtx : matrices) {
    bool fill_ok;
    const auto dt_insert = timeit([&]() {fill_ok = fill_matrix(*mtx, N);});
    print_test_result(fill_ok, "insert");
    print_test_result((mtx->nrows() == N) && (mtx->ncols() == N) && (mtx->nnz() == 3 * N - 2), "dimension");
    std::cout << "Elapsed for element access: " << dt_insert << "[ms]" << std::endl;

    Vector b;
    const auto dt_vmult = timeit([&]() {b = mtx->vmult(x);});
    print_test_result(eq(res, b), "vmult");
    std::cout << "Elapsed for vmult: " << dt_vmult << "[ms]" << std::endl;
    std::cout << "--------------------------" << std::endl;
    //mtx->print(std::cout);
    std::cout << "--------------------------" << std::endl;
  }

  return 0;
}