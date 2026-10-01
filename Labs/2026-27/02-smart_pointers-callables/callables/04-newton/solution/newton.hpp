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

// Newton's method. F and DF can be any callable double(double): function,
// function pointer, functor, lambda. Being a template, it is defined in the header.
template <typename F, typename DF>
NewtonResult newton(F const &f, DF const &df, double x0, NewtonOptions const &options = {}) {
  NewtonResult result;
  double x = x0;
  double fx = f(x);
  result.history.push_back(x);

  for (unsigned k = 0; k < options.max_iter; ++k) {
    double const dfx = df(x);
    if (dfx == 0.) // the tangent is horizontal: we cannot go on
      break;
    double const step = fx / dfx;
    x -= step;
    fx = f(x);
    result.history.push_back(x);
    result.iterations = k + 1;
    if (std::abs(fx) < options.rtol || std::abs(step) < options.stol) {
      result.converged = true;
      break;
    }
  }
  result.root = x;
  result.residual = std::abs(fx);
  return result;
}

// Returns a callable approximating f' with the centred difference
//   f'(x) ~ (f(x + h) - f(x - h)) / (2h).
// f is captured BY VALUE: the returned lambda may outlive the argument.
template <typename F>
auto centred_difference(F f, double h = 1e-6) {
  return [f, h](double x) { return (f(x + h) - f(x - h)) / (2. * h); };
}

#endif /* NEWTON_HPP */
