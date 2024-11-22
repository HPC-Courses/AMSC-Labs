#ifndef DERIVATIVES_HPP
#define DERIVATIVES_HPP

#include <functional>
#include <type_traits>

using func_t = std::function<double(double)>;


class NaiveNthDerivative {
public:
  NaiveNthDerivative(const func_t &f, double h, unsigned int deg) : 
    m_f(f), 
    m_h(h), 
    m_deg(deg) {}
  double operator()(double x) {
  }
private:
  double f_prime(double x, unsigned deg) {
  }

  const func_t m_f;
  const double m_h;
  const unsigned int m_deg;
};

#endif /* DERIVATIVES_HPP */
