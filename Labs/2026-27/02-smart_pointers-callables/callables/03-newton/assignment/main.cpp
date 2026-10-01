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

  // TODO (at home): Polynomial derivative() const
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

  // 2. (at home) TODO: functor. Find the root of p(x) = x^3 - 2x - 5 starting from x0 = 2

  // 3. TODO: lambdas. For a = 2, 3, 5 find sqrt(a) as the root of x^2 - a,
  //          with a lambda that captures a

  // 4. TODO: approximate the derivative with centred_difference(f)

  // 5. (at home) TODO: print the error |x_k - sqrt(2)| for each iterate of case 1.
  //          Is the convergence quadratic?

  // 6. (if you have time) TODO: count how many times f is evaluated in case 4,
  //          with rtol = 1e-8. Hint: a lambda that wraps f and increments a counter.
  //          How must the counter be captured?
  return 0;
}
