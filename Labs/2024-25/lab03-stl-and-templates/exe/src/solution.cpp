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

struct event_counter
{
  double cumtime;
  int    count;
};

static clock_t c_start, c_diff;
static double c_msec;
static std::map<std::string,event_counter> timing_report;

#define tic() c_start = clock ();

#define toc(X)                                                \
  c_diff = clock () - c_start;                                \
  c_msec = (double)c_diff * 1000 / (double)CLOCKS_PER_SEC;    \
  timing_report[X].cumtime += c_msec;                         \
  timing_report[X].count++;                                   \
  std::cout << X << std::endl << "Elapsed time : " << c_msec  \
    << "ms" << std::endl;    


// *use templates to make your matrix usable with different types
// *just like an std::vector can contain different elements, depending on
// *what you specified
template<typename T>
class SparseMatrix {
public:
  using Vector = std::vector<T>;
  SparseMatrix() {};
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
  size_t m_nnz = 0;
  size_t m_nrows = 0, m_ncols = 0;
};


template<typename T, template<typename,typename> class S>
class MapMatrix : public SparseMatrix<T> {
public:
  using Vector = typename SparseMatrix<T>::Vector;
  MapMatrix() {};

  void set_number_rows(size_t rr) {
    assert(SparseMatrix<T>::m_nrows == 0);
    SparseMatrix<T>::m_nrows = rr;
    m_data.resize(rr);
  };

  void finalize() {
    SparseMatrix<T>::m_nnz = 0;
    SparseMatrix<T>::m_ncols = 0;
    for (const auto & it : m_data[SparseMatrix<T>::m_nrows-1]) SparseMatrix<T>::m_ncols = it.first > SparseMatrix<T>::m_ncols ? it.first : SparseMatrix<T>::m_ncols;
    SparseMatrix<T>::m_ncols++;
    for (size_t i = 0; i < SparseMatrix<T>::m_nrows; i++) SparseMatrix<T>::m_nnz += m_data[i].size();
  };

  virtual ~MapMatrix() override = default;

  virtual Vector vmult(const Vector& x) const override {
    assert(x.size() == SparseMatrix<T>::m_ncols);
    Vector res(SparseMatrix<T>::m_ncols);
    for (size_t i = 0; i < SparseMatrix<T>::m_nrows; i++) {
      for (const auto& [j,v] : m_data[i]) {
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
    return m_data[i][j];
  }

  virtual const T& operator()(size_t i, size_t j) const override {
    return m_data[i].at(j);
  }

  const S<size_t,T>& operator()(size_t i) {
    return m_data[i];
  }

private:
  std::vector<S<size_t,T> > m_data;

protected:
  virtual void _print(std::ostream& os) const {
    for (size_t i = 0; i < m_data.size(); ++i) {
      for (const auto& [j, v] : m_data[i])
        os << i << "," << j << "," << v << std::endl;
    }
  }
};


template<typename T, template<typename,typename> class S>
class CooMatrix : public SparseMatrix<T> {
  friend class MapMatrix<T, S>;
public:
  using ijv_t = std::tuple<size_t, size_t, T>;
  using Vector = typename SparseMatrix<T>::Vector;

  virtual Vector vmult(const Vector& x) const override {
    assert(x.size() == SparseMatrix<T>::m_ncols);
    Vector res(x.size());
    for (const auto& ijv : m_tri) {
      res[std::get<0>(ijv)] += x[std::get<1>(ijv)] * std::get<2>(ijv);
    }
    return res;
  }

  virtual T& operator()(size_t i, size_t j) override {
    return std::get<2>(m_tri[find_elem(i, j)]);
  }

  virtual const T& operator()(size_t i, size_t j) const override {
    return std::get<2>(m_tri[find_elem(i, j)]);
  }

  // sligthly faster than O(log2 nnz) (complexity of std::lower_bound), but need to build a lookup table i_list
  size_t find_elem(size_t i, size_t j) const {
    const auto & start = i_list[i][0];
    size_t jj;
    for ( jj=start; jj<(start+i_list[i][1]); jj++) {
      const auto & j_cand = std::get<1>(m_tri[jj]);
      if (j==j_cand) return jj;
    }
  }

  CooMatrix(MapMatrix<T, S> A) {
    SparseMatrix<T>::m_nnz   = A.nnz  ();
    SparseMatrix<T>::m_ncols = A.ncols();
    SparseMatrix<T>::m_nrows = A.nrows();
    m_tri.resize(SparseMatrix<T>::m_nnz);
    i_list.resize(SparseMatrix<T>::m_nrows);
    size_t count = 0;
    for (size_t i = 0; i < SparseMatrix<T>::m_nrows; i++) 
    {
      const auto & A_ref_line = A(i);
      i_list[i][0] = count;
      i_list[i][1] = A_ref_line.size();
      for (const auto & it : A_ref_line) {
        m_tri[count++] = ijv_t({i, it.first, it.second});
      }
    }
  };
  virtual ~CooMatrix() override = default;
private:

  virtual void _print(std::ostream& os) const {
    for (const auto& ijv : m_tri)
      os << std::get<0>(ijv) << "," << std::get<1>(ijv) << "," << std::get<2>(ijv) << std::endl;
  }

  std::vector<ijv_t> m_tri;
  std::vector<std::array<size_t, 2> > i_list;
};



int main() {

  {
    int number_rows = 1e5;
    std::vector<double> vv(number_rows);
    for (int i=0; i<number_rows; i++) vv[i] = i;

    tic();
    MapMatrix<double, std::unordered_map > A;
    A.set_number_rows(number_rows);
    for (int i = 0; i < number_rows; i++) for (int j = std::max(i-1,0); j < std::min(i+2,number_rows); j++) A(i,j) = i == j ? -2 : 1;
    A.finalize();
    const auto res_A = A.vmult(vv);
    //A.print();
    toc("time unordered_map");

    tic();
    MapMatrix<double, std::map > B;
    B.set_number_rows(number_rows);
    for (int i = 0; i < number_rows; i++) for (int j = std::max(i-1,0); j < std::min(i+2,number_rows); j++) B(i,j) = i == j ? -2 : 1;
    B.finalize();
    const auto res_B = B.vmult(vv);
    //B.print();
    toc("time ordered");
  }


  {
    // check now the difference in time in ordered and unordered, the differnce is due when the number of elements stored in the map is "high". Please, be careful about this when choosing the best container.
    int number_rows = 1e3;

    tic();
    MapMatrix<double, std::unordered_map> A;
    A.set_number_rows(number_rows);
    for (int i = 0; i < number_rows; i++) for (int j = 0; j < 1000; j++) A(i,j) = 1;
    A.finalize();
    toc("time unordered_map");

    tic();
    MapMatrix<double, std::map> B;
    B.set_number_rows(number_rows);
    for (int i = 0; i < number_rows; i++) for (int j = 0; j < 1000; j++) B(i,j) = 1;
    B.finalize();
    toc("time ordered");
  }

  {
    int number_rows = 20;

    MapMatrix<double, std::unordered_map> A;
    A.set_number_rows(number_rows);
    for (int i = 0; i < number_rows; i++) for (int j = std::max(i-1,0); j < std::min(i+2,number_rows); j++) A(i,j) = i == j ? -2 : 1;
    A.finalize();

    CooMatrix<double, std::unordered_map> coo_mtx(A);

    //coo_mtx(0,0) = 20;
    //std::cout << coo_mtx(1,2) << std::endl;
    //coo_mtx.print();
  }

  return 0;
}