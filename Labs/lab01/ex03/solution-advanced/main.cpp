#include <iomanip>
#include <tuple>

#include "newton.hpp"
#include "auto_diff.hpp"


const std::vector<autodiff::test_t> TESTS = {
  std::make_tuple(autodiff::x, [](auto x){return x;}, [](auto x){return 1.0;}),
  std::make_tuple(autodiff::x + 1, [](auto x){return x + 1;}, [](auto x){return 1.0;}),
  std::make_tuple(autodiff::x*autodiff::x, [](auto x){return x*x;}, [](auto x){return 2*x;}),
  std::make_tuple((autodiff::x^2) + (-2.0), [](auto x){return x*x - 2.0;}, [](auto x){return 2*x;}),
  std::make_tuple(autodiff::x*autodiff::x + (-2.0), [](auto x){return x*x - 2.0;}, [](auto x){return 2*x;}),
  std::make_tuple(autodiff::cos(autodiff::sin), [](auto x){return std::cos(std::sin(x));}, [](auto x){return -std::cos(x)*std::sin(std::sin(x));}),
};


int main() {
  std::cout << std::scientific << std::setprecision(6);

  for(const auto & t : TESTS) {
    const auto err = autodiff::test(std::get<0>(t), std::get<1>(t), std::get<2>(t), -10, 10, 1000);
    std::cout << err.first << " " << err.second << std::endl;
  }

  NewtonSolver solver((autodiff::x^2) + (-2.0), 100, 1e-10, 1e-10);
  solver.solve(1.0);

  std::cout << std::scientific << std::setprecision(6);
  std::cout << "x    =    " << solver.getResult() << std::endl;
  std::cout << "r    =    " << solver.getResidual() << std::endl;
  std::cout << "dx   =    " << solver.getStep() << std::endl;
  std::cout << "iter =    " << solver.getIter() << std::endl;

  return 0;
}