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

template<typename T>
class SparseMatrix {
public:
  using Vector = std::vector<T>;
  SparseMatrix() : m_nnz(0), m_nrows(0), m_ncols(0) {};
  size_t nrows() const { return m_nrows; }
  size_t ncols() const { return m_ncols; }
  size_t nnz() const { return m_nnz; }

  void print(std::ostream& os = std::cout) const {
    os << "nrows: " << m_nrows << " | ncols:" << m_ncols << " | nnz: " << m_nnz << std::endl;
    _print(os);
  };

  virtual Vector vmult(const Vector& v) const = 0;
  virtual const T& operator()(size_t i, size_t j) const = 0;
  virtual T& operator()(size_t i, size_t j) = 0;
  virtual ~SparseMatrix() = default;

protected:
  virtual void _print(std::ostream& os) const = 0;
  size_t m_nnz;
  size_t m_nrows, m_ncols;
};

template<typename T>
class MapMatrix : public SparseMatrix<T> {
public:
  using Vector = typename SparseMatrix<T>::Vector;
  virtual Vector vmult(const Vector& x) const override {
    assert(x.size() == SparseMatrix<T>::m_ncols);
    Vector res(x.size());
    for (size_t i = 0; i < m_data.size(); ++i) {
      for (const auto& [j, v] : m_data[i]) {
        res[i] += x[j] * v;
      }
    }
    return res;
  }

  virtual T& operator()(size_t i, size_t j) override {
    if (m_data.size() < i + 1) {
      m_data.resize(i + 1);
      SparseMatrix<T>::m_nrows = i + 1;
    }
    const auto it = m_data[i].find(j);
    if (it == m_data[i].end()) {
      SparseMatrix<T>::m_ncols = std::max(SparseMatrix<T>::m_ncols, j + 1);
      SparseMatrix<T>::m_nnz++;
      return (*m_data[i].emplace(j, 0).first).second;
    }
    return (*it).second;
  }
  virtual const T& operator()(size_t i, size_t j) const override {
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
  std::vector<std::map<size_t, T>> m_data;
};


// fill a tridiagonal matrix
template<typename T>
void fill_matrix(SparseMatrix<T>& mm, size_t N) {
  mm(N - 1, N - 2) = 1;
  mm(N - 1, N - 1) = -2;
  for (size_t i = N - 2; i > 0; --i) {
    mm(i, i - 1) = 1;
    mm(i, i) = -2;
    mm(i, i + 1) = 1;
  }
  mm(0, 0) = -2;
  mm(0, 1) = 1;
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

// utility for printing the result of a test
void print_test_result(bool r, const std::string& test_name) {
  std::cout << test_name << " test: " << (r ? "PASSED" : "FAILED") << std::endl;
}

int main() {
  constexpr size_t N = 5; // size of the matrix

  using elem_t = double;
  MapMatrix<elem_t> mtx;
  MapMatrix<elem_t>::Vector x(N), res(N); // 'res' is the vmult result of matrix * x
  std::iota(x.begin(), x.end(), 0);
  res[0] = 1;
  res[N - 1] = -static_cast<elem_t>(N);
  
  {
    using namespace std::chrono;
    const auto t0 = high_resolution_clock::now();
    fill_matrix(mtx, N);
    const auto t1 = high_resolution_clock::now();
    const auto dt_insert = duration_cast<milliseconds>(t1 - t0).count();
    print_test_result((mtx.nrows() == N) && (mtx.ncols() == N) && (mtx.nnz() == 3 * N - 2), "dimension");
    std::cout << "Elapsed for element access: " << dt_insert << "[ms]" << std::endl;
  }

  SparseMatrix<elem_t>::Vector b;
  {
    using namespace std::chrono;
    const auto t0 = high_resolution_clock::now();
    b = mtx.vmult(x);
    const auto t1 = high_resolution_clock::now();
    const auto dt_vmult = duration_cast<milliseconds>(t1 - t0).count();
    print_test_result(eq(res, b), "vmult");
    std::cout << "Elapsed for vmult: " << dt_vmult << "[ms]" << std::endl;
  }
  std::cout << "--------------------------" << std::endl;

  if(N < 10)
    mtx.print(std::cout);

  return 0;
}