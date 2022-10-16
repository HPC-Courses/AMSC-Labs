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


int main() {
  CooMatrix m;
  m(0, 0) = 1;
  m(1, 1) = 1;
  m.print(std::cout);
  const auto x = m.vmult({ {2, 3} });
  std::cout << x[0] << " " << x[1] << std::endl;
  return 0;
}