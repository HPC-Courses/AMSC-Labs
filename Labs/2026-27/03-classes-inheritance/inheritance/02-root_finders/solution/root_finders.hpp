#ifndef ROOT_FINDERS_HPP
#define ROOT_FINDERS_HPP

#include <functional>
#include <string>
#include <vector>

// The same two structs of Lab 2, renamed: they are no longer specific to Newton
struct RootFinderOptions {
  unsigned max_iter = 100;
  double rtol = 1e-12; // tolerance on the residual |f(x_k)|
  double stol = 1e-12; // tolerance on the step |x_k - x_{k-1}|
};

struct RootFinderResult {
  double root = 0.;
  unsigned iterations = 0;
  double residual = 0.;
  bool converged = false;
  std::vector<double> history;
};

// Abstract base class: what all the root finders have in common
class RootFinder {
public:
  // The function must be STORED as a data member, and all the derived classes must
  // share the same base: a template parameter is not an option, std::function is
  using Function = std::function<double(double)>;

  RootFinder(const Function &f, const RootFinderOptions &options) : f_{f}, options_{options} {}
  virtual ~RootFinder() = default;

  virtual RootFinderResult solve() const = 0;
  virtual std::string name() const = 0;

protected:
  // protected: accessible to the derived classes, not to the users
  Function f_;
  RootFinderOptions options_;
};

class Newton final : public RootFinder {
public:
  Newton(const Function &f, const Function &df, double x0, const RootFinderOptions &options = {})
      : RootFinder{f, options}, df_{df}, x0_{x0} {}
  RootFinderResult solve() const override;
  std::string name() const override { return "Newton"; }

private:
  Function df_;
  double x0_;
};

// x_{k+1} = x_k - f(x_k) (x_k - x_{k-1}) / (f(x_k) - f(x_{k-1}))
class Secant final : public RootFinder {
public:
  Secant(const Function &f, double x0, double x1, const RootFinderOptions &options = {})
      : RootFinder{f, options}, x0_{x0}, x1_{x1} {}
  RootFinderResult solve() const override;
  std::string name() const override { return "Secant"; }

private:
  double x0_, x1_;
};

// Halves [a, b] keeping the half where f changes sign
class Bisection final : public RootFinder {
public:
  // Throws std::invalid_argument if f(a) and f(b) have the same sign
  Bisection(const Function &f, double a, double b, const RootFinderOptions &options = {});
  RootFinderResult solve() const override;
  std::string name() const override { return "Bisection"; }

private:
  double a_, b_;
};

#endif /* ROOT_FINDERS_HPP */
