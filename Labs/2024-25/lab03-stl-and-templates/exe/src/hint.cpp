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
  };


  virtual Vector vmult(const Vector& v) const = 0;
  virtual const T& operator()(size_t i, size_t j) const = 0;
  virtual T& operator()(size_t i, size_t j) = 0;
  virtual ~SparseMatrix() = default;

protected:
  size_t m_nnz = 0;
  size_t m_nrows = 0, m_ncols = 0;
};

int main() {
  tic();
  //SparseMatrix<double> A;
  //A.print();
  toc("first part!!");



  return 0;
}