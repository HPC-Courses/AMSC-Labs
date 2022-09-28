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
    fun(fun_),
    dfun(dfun_),
    n_max_it(n_max_it_),
    tol_fun(tol_fun_),
    tol_x(tol_x_),
    x(0),
    df_dx(0),
    dx(0),
    res(0),
    iter(0) {
  }

  void solve(const double x0) {
    x.resize(1);
    x.reserve(n_max_it + 1);
    x[0] = x0;
    for (iter = 0; iter < n_max_it; ++iter) {
      res = fun(x[iter]);

      if (std::abs(res) < tol_fun)
        break;

      df_dx = dfun(x[iter]);

      dx = -res / df_dx;
      x.push_back(x[iter] + dx);

      if (std::abs(dx) < tol_x)
        break;
    }
  }

  double getResult() const {
    return x[x.size() - 1];
  };

  double getStep() const {
    return dx;
  };

  const std::vector<double>& getHistory() const {
    return x;
  }

  double getResidual() const {
    return res;
  };

  unsigned int getIter() const {
    return iter;
  };

private:
  std::function<double(double)> fun;
  std::function<double(double)> dfun;

  const unsigned int n_max_it;
  const double       tol_fun;
  const double       tol_x;

  std::vector<double> x;
  double              df_dx;
  double              dx;
  double              res;
  unsigned int        iter;
};

#endif /* NEWTON_H */