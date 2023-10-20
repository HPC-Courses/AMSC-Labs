#ifndef NEWTON_H
#define NEWTON_H

#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <vector>

class NewtonSolver {
public:
  NewtonSolver(
    const std::function<double(double)>& fun_,
    const std::function<double(double)>& dfun_,
    const unsigned int n_max_it_ = 100,
    const double tol_fun_ = std::numeric_limits<double>::epsilon(),
    const double tol_x_ = std::numeric_limits<double>::epsilon()
  )
    :
    m_fun(fun_),
    m_dfun(dfun_),
    m_n_max_it(n_max_it_),
    m_tol_fun(tol_fun_),
    m_tol_x(tol_x_),
    m_x(0),
    m_df_dx(0),
    m_dx(0),
    m_res(0),
    m_iter(0) {
  }

  void solve(const double x0);

  double getResult() const {
    return m_x.back();
  };

  double getStep() const {
    return m_dx;
  };

  const std::vector<double>& getHistory() const {
    return m_x;
  }

  double getResidual() const {
    return m_res;
  };

  unsigned int getIter() const {
    return m_iter;
  };

private:
  std::function<double(double)> m_fun;
  std::function<double(double)> m_dfun;

  const unsigned int m_n_max_it;
  const double       m_tol_fun;
  const double       m_tol_x;

  std::vector<double> m_x;
  double              m_df_dx;
  double              m_dx;
  double              m_res;
  unsigned int        m_iter;
};

#endif /* NEWTON_H */