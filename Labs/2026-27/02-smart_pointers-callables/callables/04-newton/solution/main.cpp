#include "newton.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

// 1. Free functions
double f(double x) { return x * x - 2.; }
double df(double x) { return 2. * x; }

// 2. A functor with state: a polynomial p(x) = c_0 + c_1 x + ... + c_n x^n
struct Polynomial {
  std::vector<double> coeffs;

  // Horner's rule
  double operator()(double x) const {
    double p = 0.;
    for (auto it = coeffs.rbegin(); it != coeffs.rend(); ++it)
      p = p * x + *it;
    return p;
  }

  Polynomial derivative() const {
    Polynomial d;
    for (std::size_t i = 1; i < coeffs.size(); ++i)
      d.coeffs.push_back(static_cast<double>(i) * coeffs[i]);
    return d;
  }
};

void report(std::string const &title, NewtonResult const &r) {
  std::cout << std::setw(28) << std::left << title << " root = " << std::setprecision(15) << r.root
            << ", iterations = " << r.iterations << ", residual = " << std::setprecision(3)
            << r.residual << (r.converged ? "" : "  NOT CONVERGED") << std::endl;
}

int main() {
  double const x0 = 1.;

  // 1. functions
  report("functions", newton(f, df, x0));

  // 2. functor: p(x) = x^3 - 2x - 5 (the historical example of Newton himself)
  Polynomial p{{-5., -2., 0., 1.}};
  report("functor (x^3-2x-5)", newton(p, p.derivative(), 2.));

  // 3. lambdas capturing a parameter: sqrt(a) as root of x^2 - a
  for (double a : {2., 3., 5.}) {
    auto fa = [a](double x) { return x * x - a; };
    auto dfa = [](double x) { return 2. * x; };
    report("lambda, sqrt(" + std::to_string(static_cast<int>(a)) + ")", newton(fa, dfa, x0));
  }

  // 4. derivative by finite differences: a function returning a lambda
  report("finite differences", newton(f, centred_difference(f), x0));

  // 5. convergence history: the error roughly squares at every iteration
  auto const r = newton(f, df, x0);
  std::cout << "\nerror history, exact derivative:" << std::endl;
  for (double x : r.history)
    std::cout << "  " << std::scientific << std::abs(x - std::sqrt(2.)) << std::endl;

  // 6. counting the evaluations of f: a side effect on a variable captured by reference.
  //    A counter captured by value in a mutable lambda would NOT compile here:
  //    newton takes f as F const&, and a mutable lambda has a non-const operator().
  unsigned n_eval = 0;
  auto f_counted = [&n_eval](double x) {
    ++n_eval;
    return f(x);
  };
  NewtonOptions options; // the defaults, except for the tolerance on the residual
  options.rtol = 1e-8;
  auto const r6 = newton(f_counted, centred_difference(f_counted), x0, options);
  std::cout << std::defaultfloat << "\nwith finite differences and rtol = 1e-8: " << r6.iterations
            << " iterations, " << n_eval << " evaluations of f" << std::endl;
  return 0;
}
