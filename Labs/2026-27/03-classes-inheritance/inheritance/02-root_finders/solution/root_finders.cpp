#include "root_finders.hpp"

#include <cmath>
#include <stdexcept>

RootFinderResult Newton::solve() const {
  RootFinderResult result;
  double x = x0_;
  double fx = f_(x);
  result.history.push_back(x);
  for (unsigned k = 0; k < options_.max_iter; ++k) {
    double const dfx = df_(x);
    if (dfx == 0.)
      break;
    double const step = fx / dfx;
    x -= step;
    fx = f_(x);
    result.history.push_back(x);
    result.iterations = k + 1;
    if (std::abs(fx) < options_.rtol || std::abs(step) < options_.stol) {
      result.converged = true;
      break;
    }
  }
  result.root = x;
  result.residual = std::abs(fx);
  return result;
}

RootFinderResult Secant::solve() const {
  RootFinderResult result;
  double x_prev = x0_, x = x1_;
  double f_prev = f_(x_prev), fx = f_(x);
  result.history = {x_prev, x};
  for (unsigned k = 0; k < options_.max_iter; ++k) {
    if (fx == f_prev) // horizontal secant
      break;
    double const step = fx * (x - x_prev) / (fx - f_prev);
    x_prev = x;
    f_prev = fx;
    x -= step;
    fx = f_(x);
    result.history.push_back(x);
    result.iterations = k + 1;
    if (std::abs(fx) < options_.rtol || std::abs(step) < options_.stol) {
      result.converged = true;
      break;
    }
  }
  result.root = x;
  result.residual = std::abs(fx);
  return result;
}

Bisection::Bisection(const Function &f, double a, double b, const RootFinderOptions &options)
    : RootFinder{f, options}, a_{a}, b_{b} {
  if (f_(a_) * f_(b_) > 0.)
    throw std::invalid_argument("f(a) and f(b) must have opposite signs");
}

RootFinderResult Bisection::solve() const {
  RootFinderResult result;
  double a = a_, b = b_;
  double fa = f_(a);
  double x = a, fx = fa;
  for (unsigned k = 0; k < options_.max_iter; ++k) {
    x = 0.5 * (a + b);
    fx = f_(x);
    result.history.push_back(x);
    result.iterations = k + 1;
    if (std::abs(fx) < options_.rtol || 0.5 * (b - a) < options_.stol) {
      result.converged = true;
      break;
    }
    if (fa * fx < 0.)
      b = x;
    else {
      a = x;
      fa = fx;
    }
  }
  result.root = x;
  result.residual = std::abs(fx);
  return result;
}
