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
    // assert x.size() == m_ncols
    // allocate memory for result and initialize to 0
    // loop over each element of the matrix and add contribute to result
  }
  virtual double& operator()(size_t i, size_t j) override {
    // find if the element is present with 'it = std::lower_bound(...)'
    // if 'it' points the end then we add a new element
    // else if 'it' does not points the element we are looking for means it is pointing
    // exactly where we should add the new elemnt, so we add one element to the end
    // and shift the elements after (*it) one place forward
    // we update nnz, nrows and ncols accordingly to what we have done
    // then return a reference to the element
  }
  virtual const double& operator()(size_t i, size_t j) const override {
    // find the element with std::lower_bound and return it with no checks
  }

  virtual ~CooMatrix() override = default;
private:
  virtual void _print(std::ostream& os) const {
    // loop over each elemnt and print it
  }
  // define the type 'ijv_t' that is the type of the tuple containg the row index, 
  // col index and value of an element
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