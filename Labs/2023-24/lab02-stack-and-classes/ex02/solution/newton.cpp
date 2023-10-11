#include "newton.hpp"

void NewtonSolver::solve(const double x0) {
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