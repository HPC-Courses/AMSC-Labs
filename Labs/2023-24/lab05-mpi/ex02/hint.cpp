#include <mpi.h>

#include <array>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <numbers>

template <size_t N>
std::array<double, N> compute_gradient(
    const std::function<double(const std::array<double, N> &)> &f,
    const std::array<double, N> &x, double h) {
  // initialize gradient
  // FILL HERE

  // get rank and size
  // FILL HERE

  // subdivide computations by processor
  for (/*FILL HERE*/) {
    auto x_plus_h(x), x_minus_h(x);
    x_plus_h[i] += h;
    x_minus_h[i] -= h;
    gradient[i] = (f(x_plus_h) - f(x_minus_h)) / 2.0 / h;
  }

  // send result to main proc
  for (size_t i = 0; i < N; ++i) {  // for each component of the gradient
    const int belongs_to_proc =
        /*FILL HERE: compute to which proc it belongs to*/;
    if (belongs_to_proc /*!= 0*/) {  // then we need to send it to proc 0
      if (/*FILL HERE: belong to us*/) {
        // FILL HERE: we send it to proc 0
      } else if (/*FILL HERE: we are proc 0*/) {
        // FILL HERE: we receive the data
      }
    }
  }
  return gradient;
}

int main(int argc, char *argv[]) {
  // init MPI, get rank and size
  // FILL HERE

  // define types
  using Vector = std::array<double, 4>;
  using Function = std::function<double(const Vector &)>;
  // define the field f of which we compute the gradient at the point where we
  // evaluate it
  const Function f = [](auto v) {
    return std::sin(v[0]) + std::exp(v[1]) + std::cos(v[2]) +
           2 * v[3] * v[3] * v[3];
  };
  const Vector y = {0, 1, std::numbers::pi, 2};
  // define the correct value of the gradient
  const Vector g_true = {1, std::exp(1), 0, 24};
  // compute the gradient with our function
  const Vector g = compute_gradient(f, y, 1e-8);

  // check if the solution is correct
  if (/*FILL HERE*/) {
    std::cout << std::setprecision(4) << std::scientific;
    for (size_t i = 0; i < g.size(); ++i) {
      const auto err = std::abs(g[i] - g_true[i]);
      std::cout << i << " | " << g[i] << (err < 1e-6 ? " PASS" : " FAIL")
                << std::endl;
    }
  }
  // FILL HERE: finalize
  return 0;
}