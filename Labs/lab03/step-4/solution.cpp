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

using elem_t = double;
using Vector = std::vector<elem_t>;

class SparseMatrix {
public:
  SparseMatrix() : m_nnz(0), m_nrows(0), m_ncols(0) {};
  inline size_t nrows() { return m_nrows; }
  inline size_t ncols() { return m_ncols; }
  inline size_t nnz() { return m_nnz; }

  void print(std::ostream& os) {
    std::cout << "nrows: " << m_nrows << " | ncols:" << m_ncols << " | nnz: " << m_nnz << std::endl;
    _print(os);
  };

  virtual Vector vmult(const Vector& v) const = 0;
  virtual const elem_t& operator()(size_t i, size_t j) const = 0;
  virtual elem_t& operator()(size_t i, size_t j) = 0;
  virtual ~SparseMatrix() = default;

protected:
  virtual void _print(std::ostream& os) const = 0;
  size_t m_nnz;
  size_t m_nrows, m_ncols;
};

class MapMatrix : public SparseMatrix {
public:
  virtual Vector vmult(const Vector& x) const override {
    assert(x.size() == m_ncols);
    Vector res(x.size());
    for (size_t i = 0; i < m_data.size(); ++i) {
      for (const auto& [j, v] : m_data[i]) {
        res[i] += x[j] * v;
      }
    }
    return res;
  }

  virtual double& operator()(size_t i, size_t j) override {
    if (m_data.size() < i + 1) {
      m_data.resize(i + 1);
      m_nrows = i + 1;
    }
    const auto it = m_data[i].find(j);
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
  std::vector<std::map<size_t, elem_t>> m_data;
};


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
    const auto it = find_elem(i, j);
    const size_t idx = it - m_data.begin();
    if (it == m_data.cend()) {
      m_data.emplace_back(std::forward_as_tuple(i, j, 0));
      m_nnz++;
      m_ncols = std::max(m_ncols, j + 1);
      m_nrows = std::max(m_nrows, i + 1);
      return std::get<2>(m_data.back());
    }
    if ((std::get<0>(*it) != i) || (std::get<1>(*it) != j)) {
      m_data.push_back(m_data.back());
      for (size_t k = m_data.size() - 2; k > idx; --k)
        m_data[k] = m_data[k - 1];
      m_data[idx] = std::make_tuple(i, j, 0);
      m_nnz++;
      m_ncols = std::max(m_ncols, j + 1);
      m_nrows = std::max(m_nrows, i + 1);
    }
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
  inline std::vector<ijv_t>::const_iterator find_elem(size_t i, size_t j) const {
    return std::lower_bound(
      m_data.begin(),
      m_data.end(),
      std::make_pair(i, j),
      [](const ijv_t& x, const auto& value) {
        return (std::get<0>(x) < value.first) || ((std::get<0>(x) == value.first) && (std::get<1>(x) < value.second));
      });
  }
  std::vector<ijv_t> m_data;
};


// fill a tridiagonal matrix
void fill_matrix(SparseMatrix& mm, size_t N) {
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

// time a function execution time
auto timeit(const std::function<void()>& f) {
  using namespace std::chrono;
  const auto t0 = high_resolution_clock::now();
  f();
  const auto t1 = high_resolution_clock::now();
  return duration_cast<milliseconds>(t1 - t0).count();
}

// utility for printing the result of a test
void print_test_result(bool r, const std::string& test_name) {
  std::cout << test_name << " test: " << (r ? "PASSED" : "FAILED") << std::endl;
}

int main() {
  constexpr size_t N = 5; // size of the matrix
  Vector x(N), res(N); // 'res' is the vmult result of matrix * x
  std::iota(x.begin(), x.end(), 0);
  res[0] = 1;
  res[N - 1] = -elem_t(N);

  std::vector<std::shared_ptr<SparseMatrix>> matrices = {
    std::make_shared<MapMatrix>(),
    std::make_shared<CooMatrix>(),
  };

  // loop over every matrix and test and benckmark it
  for (auto& mtx : matrices) {
    const auto dt_insert = timeit([&]() {fill_matrix(*mtx, N);});
    print_test_result((mtx->nrows() == N) && (mtx->ncols() == N) && (mtx->nnz() == 3 * N - 2), "dimension");
    std::cout << "Elapsed for element access: " << dt_insert << "[ms]" << std::endl;

    Vector b;
    const auto dt_vmult = timeit([&]() {b = mtx->vmult(x);});
    print_test_result(eq(res, b), "vmult");
    std::cout << "Elapsed for vmult: " << dt_vmult << "[ms]" << std::endl;
    std::cout << "--------------------------" << std::endl;
    mtx->print(std::cout);
    std::cout << "--------------------------" << std::endl;
  }

  return 0;
}