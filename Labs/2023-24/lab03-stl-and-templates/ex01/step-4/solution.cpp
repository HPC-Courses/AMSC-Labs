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
class CooMatrix;

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

  CooMatrix<T> to_coo() const;
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

template<typename T>
class CooMatrix : public SparseMatrix<T> {
  friend class MapMatrix<T>;
public:
  using ijv_t = std::tuple<size_t, size_t, T>;
  using Vector = typename SparseMatrix<T>::Vector;

  virtual Vector vmult(const Vector& x) const override {
    assert(x.size() == SparseMatrix<T>::m_ncols);
    Vector res(x.size());
    for (const auto& ijv : m_data) {
      res[std::get<0>(ijv)] += x[std::get<1>(ijv)] * std::get<2>(ijv);
    }
    return res;
  }
  virtual T& operator()(size_t i, size_t j) override {
    return std::get<2>(m_data[find_elem(i, j) - m_data.begin()]);
  }
  virtual const T& operator()(size_t i, size_t j) const override {
    return std::get<2>(*find_elem(i, j));
  }

  virtual ~CooMatrix() override = default;
private:
  CooMatrix(const std::vector<ijv_t> &data, size_t nrows, size_t ncols) : m_data(data) {
    SparseMatrix<T>::m_nrows = nrows;
    SparseMatrix<T>::m_ncols = ncols;
  }

  virtual void _print(std::ostream& os) const {
    for (const auto& ijv : m_data)
      os << std::get<0>(ijv) << "," << std::get<1>(ijv) << "," << std::get<2>(ijv) << std::endl;
  }
  
  // utility to find element among the data
  // if we keep the Vector sorted we can find the element in O(log nnz) with std::lower_bound
  // instead of O(nnz) when using std::find_if
  typename std::vector<ijv_t>::const_iterator find_elem(size_t i, size_t j) const {
    // We employ std::lower_bound to search in a sorted range.
    // Returns an iterator pointing to the first element in the range [begin, end) 
    // such that element < std::make_pair(i, j) is false, or end if no such element is found.
    // Since we are comparing a tuple of size three and a std::pair,
    // we must implement a custom comparison operator that defines 
    // `<` in the expression `element < std::make_pair(i, j)`
    const auto it = std::lower_bound(
      m_data.begin(),
      m_data.end(),
      std::make_pair(i, j), // the value we are looking for
      [](const ijv_t& x, const auto& value) { // the `<` operator
        return (std::get<0>(x) < value.first) || ((std::get<0>(x) == value.first) && (std::get<1>(x) < value.second));
      }
    );
    if( (it == m_data.cend()) || (std::get<0>(*it) != i) || (std::get<1>(*it) != j) ) {
      std::cerr << "Error: accessing an element of a COO matrix that is not present" << std::endl;
      std::exit(-1);
    }
    return it;
  }

  std::vector<ijv_t> m_data;
};


template<typename T>
CooMatrix<T> MapMatrix<T>::to_coo() const {
  decltype(CooMatrix<T>::m_data) data;
  for(size_t i = 0; i < m_data.size(); ++i) {
    const auto &row = m_data[i];
    for(const auto &[j, val] : row) {
      data.push_back(std::make_tuple(i, j, val));
    }
  }
  return CooMatrix<T>(data, SparseMatrix<T>::m_nrows, SparseMatrix<T>::m_ncols);
}


int main() {
  constexpr size_t N = 5; // size of the matrix

  using elem_t = double;
  MapMatrix<elem_t> mtx;
  SparseMatrix<elem_t>::Vector x(N), res(N); // 'res' is the vmult result of matrix * x
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
    std::cout << "Elapsed for fill map matrix: " << dt_insert << "[ms]" << std::endl;
  }

  CooMatrix<elem_t> coo_mtx = mtx.to_coo();

  SparseMatrix<elem_t>::Vector b;
  {
    using namespace std::chrono;
    const auto t0 = high_resolution_clock::now();
    b = mtx.vmult(x);
    const auto t1 = high_resolution_clock::now();
    const auto dt_vmult = duration_cast<milliseconds>(t1 - t0).count();
    print_test_result(eq(res, b), "vmult map matrix");
    std::cout << "Elapsed for vmult map matrix: " << dt_vmult << "[ms]" << std::endl;
  }

  {
    using namespace std::chrono;
    const auto t0 = high_resolution_clock::now();
    b = coo_mtx.vmult(x);
    const auto t1 = high_resolution_clock::now();
    const auto dt_vmult = duration_cast<milliseconds>(t1 - t0).count();
    print_test_result(eq(res, b), "vmult coo matrix");
    std::cout << "Elapsed for vmult coo matrix: " << dt_vmult << "[ms]" << std::endl;
  }

  
  if(N < 10) {
    std::cout << "--------------------------" << std::endl;
    mtx.print(std::cout);
    coo_mtx.print(std::cout);
  }

  return 0;
}