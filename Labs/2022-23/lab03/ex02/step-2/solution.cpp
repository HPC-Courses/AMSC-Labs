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
  size_t nrows() const { return m_nrows; }
  size_t ncols() const { return m_ncols; }
  size_t nnz() const { return m_nnz; }

  void print(std::ostream& os) const {
    os << "nrows: " << m_nrows << " | ncols:" << m_ncols << " | nnz: " << m_nnz << std::endl;
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

int main() {
  MapMatrix m;
  m(0, 0) = 1;
  m(1, 1) = 1;
  m.print(std::cout);
  const auto x = m.vmult({{2, 3}});
  std::cout << x[0] << " " << x[1] << std::endl;
  return 0;
}