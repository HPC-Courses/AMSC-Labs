#include "newton.hpp"

void NewtonSolver::solve(const double x0) {
    m_x.resize(1);
    m_x.reserve(m_n_max_it + 1);
    m_x[0] = x0;
    for (m_iter = 0; m_iter < m_n_max_it; ++m_iter) {
      m_res = m_fun(m_x[m_iter]);

      if (std::abs(m_res) < m_tol_fun)
        break;

      m_df_dx = m_dfun(m_x[m_iter]);

      m_dx = -m_res / m_df_dx;
      m_x.push_back(m_x[m_iter] + m_dx);

      if (std::abs(m_dx) < m_tol_x)
        break;
    }
}