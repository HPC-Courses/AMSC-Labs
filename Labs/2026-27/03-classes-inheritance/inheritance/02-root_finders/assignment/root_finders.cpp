#include "root_finders.hpp"

#include <cmath>
#include <stdexcept>

RootFinderResult Newton::solve() const {
  RootFinderResult result;
  // TODO: the body of newton() of Lab 2. What changes, now that f, df, x0 and the
  //       options are data members?
  result.root = x0_;
  return result;
}

// TODO: Secant::solve()

// TODO: Bisection::Bisection(...) and Bisection::solve()
