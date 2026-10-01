#ifndef NEWTON_HPP
#define NEWTON_HPP

#include <cmath>
#include <vector>

// All the parameters of the method in one place, with sensible defaults
struct NewtonOptions {
  unsigned max_iter = 100;
  double rtol = 1e-12; // tolerance on the residual |f(x_k)|
  double stol = 1e-12; // tolerance on the step |x_k - x_{k-1}|
};

struct NewtonResult {
  double root = 0.;
  unsigned iterations = 0;
  double residual = 0.;
  bool converged = false;
  std::vector<double> history; // x_0, x_1, ..., x_k
};

// Newton's method.
// TODO: f and df are function pointers. Make newton a function template, so that
//       it also accepts functors and lambdas with captures.
inline NewtonResult newton(double (*f)(double), double (*df)(double), double x0,
                           NewtonOptions const &options = {}) {
  NewtonResult result;
  // TODO: implement x_{k+1} = x_k - f(x_k) / f'(x_k), storing the iterates in
  //       result.history, and stop when
  //       - |f(x_k)| < rtol, or
  //       - |x_k - x_{k-1}| < stol, or
  //       - max_iter iterations have been done.
  //       What if f'(x_k) == 0?
  (void)f, (void)df, (void)options; // remove this line when you use them
  result.root = x0;
  return result;
}

// TODO: write a function template centred_difference(f, h = 1e-6) that RETURNS a
//       callable approximating f' with  f'(x) ~ (f(x + h) - f(x - h)) / (2h).
//       How should f be captured?

#endif /* NEWTON_HPP */
