#include "root_finders.hpp"

#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

int main() {
  auto f = [](double x) { return x * x - 2.; };
  auto df = [](double x) { return 2. * x; };

  std::vector<std::unique_ptr<RootFinder>> solvers;
  solvers.push_back(std::make_unique<Newton>(f, df, 1.));
  // TODO: uncomment when Secant and Bisection are ready
  // solvers.push_back(std::make_unique<Secant>(f, 1., 2.));
  // solvers.push_back(std::make_unique<Bisection>(f, 0., 2.));

  // An invalid interval: the constructor refuses to build the object
  // try {
  //   solvers.push_back(std::make_unique<Bisection>(f, 2., 3.));
  // } catch (const std::invalid_argument &e) {
  //   std::cerr << "Bisection on [2, 3] not created: " << e.what() << std::endl;
  // }

  // The same loop for all the methods: solve() is resolved at run time
  for (const auto &s : solvers) {
    auto const r = s->solve();
    std::cout << std::setw(10) << std::left << s->name() << " root = " << std::setprecision(15) << r.root
              << ", iterations = " << std::setw(3) << r.iterations << (r.converged ? "" : " NOT CONVERGED")
              << std::endl;
  }
  return 0;
}
