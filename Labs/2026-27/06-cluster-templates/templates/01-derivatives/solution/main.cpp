#include "derivatives.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>

// The error of the N-th derivative of f in x, with spacing h
template <unsigned N, typename F>
double error(const F &f, double x, double h, double exact) {
  const NthDerivative<N, F> derivative{f, h};
  return std::abs(derivative(x) - exact);
}

// If the error goes as h^p, halving h divides it by 2^p
template <unsigned N, typename F>
void convergence(const F &f, double x, double exact) {
  const double error_h = error<N>(f, x, 0.01, exact);
  const double error_h2 = error<N>(f, x, 0.005, exact);
  std::cout << N << "   " << error_h << "   " << error_h2 << "   " << error_h / error_h2 << std::endl;
}

int main() {
  const auto f = [](auto x) { return std::sin(x); };
  const double x = 1.;
  const double exact[] = {std::sin(x), std::cos(x), -std::sin(x), -std::cos(x), std::sin(x)};

  std::cout << std::setprecision(3);
  std::cout << "Steps 2 and 3: f = sin(x), x = 1, h = 0.01" << std::endl;
  std::cout << "N   error(h)   error(h/2)   ratio" << std::endl;
  convergence<1>(f, x, exact[1]);
  convergence<2>(f, x, exact[2]);
  convergence<3>(f, x, exact[3]);
  convergence<4>(f, x, exact[4]);

  std::cout << std::endl << "Step 4: make_derivative" << std::endl;
  const auto d2 = make_derivative<2>(f, 1e-3);
  const auto d2_float = make_derivative<2>(f, 1e-3f);
  std::cout << "double: " << d2(1.) << ", float: " << d2_float(1.f) << ", exact: " << exact[2] << std::endl;

  std::cout << std::endl << "Step 5: how small should h be?" << std::endl;
  std::cout << "h        N = 2      N = 4" << std::endl;
  for (double h : {1e-1, 1e-2, 1e-3, 1e-4, 1e-5, 1e-6})
    std::cout << h << "   " << error<2>(f, x, h, exact[2]) << "   " << error<4>(f, x, h, exact[4]) << std::endl;

  return 0;
}
