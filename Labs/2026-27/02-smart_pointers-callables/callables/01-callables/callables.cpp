/*
The same function, f(x) = a*x^2 - 2, written as every kind of callable.

compile with
  g++ -Wall -std=c++20 callables.cpp -o callables
and run with
  ./callables
*/

#include <functional>
#include <iostream>
#include <type_traits>
#include <vector>

// 1. A free function
double f_free(double x) { return x * x - 2.; }

// 2. A function object (functor): a class with operator(). It can carry state
struct Parabola {
  double a = 1.;
  double operator()(double x) const { return a * x * x - 2.; }
};

// 3. A class with a member function
struct Model {
  double a = 1.;
  double eval(double x) const { return a * x * x - 2.; }
};

// A generic "algorithm": it accepts ANY callable F with signature double(double).
// The concrete type of f is known at compile time.
template <typename F>
double evaluate_at_two(F const &f) {
  return f(2.);
}

// The same algorithm with a fixed signature: only functions (and captureless lambdas)
double evaluate_at_two_ptr(double (*f)(double)) { return f(2.); }

int main() {
  // Function pointer
  double (*pf)(double) = f_free; // '&f_free' is equivalent
  std::cout << "function pointer: " << pf(2.) << std::endl;

  // Functor, with state
  Parabola p{3.};
  std::cout << "functor:          " << p(2.) << std::endl;
  p.a = 1.;
  std::cout << "functor, a=1:     " << p(2.) << std::endl;

  // Lambda: the compiler writes a functor like Parabola for us
  double a = 3.;
  auto lambda = [a](double x) { return a * x * x - 2.; };
  std::cout << "lambda:           " << lambda(2.) << std::endl;
  a = 1.; // does NOT change the lambda: a was captured by value
  std::cout << "lambda, after a=1:" << lambda(2.) << std::endl;
  auto lambda_ref = [&a](double x) { return a * x * x - 2.; };
  a = 3.;
  std::cout << "lambda by ref:    " << lambda_ref(2.) << std::endl;

  // Pointer to member function: needs an object. std::invoke gives a uniform syntax
  Model m{3.};
  double (Model::*pmf)(double) const = &Model::eval;
  std::cout << "member pointer:   " << (m.*pmf)(2.) << std::endl;
  std::cout << "std::invoke:      " << std::invoke(pmf, m, 2.) << std::endl;

  // The template accepts all of them
  std::cout << "template:         " << evaluate_at_two(f_free) << " " << evaluate_at_two(p) << " "
            << evaluate_at_two(lambda) << std::endl;

  // The function pointer version accepts only functions and captureless lambdas
  std::cout << "pointer version:  " << evaluate_at_two_ptr(f_free) << " "
            << evaluate_at_two_ptr([](double x) { return x * x - 2.; }) << std::endl;
  // evaluate_at_two_ptr(lambda); // ERROR: a capturing lambda is not a function pointer. Try it!

  // std::function: one type for all of them (type erasure), e.g. to store them in a vector
  std::vector<std::function<double(double)>> fs{f_free, p, lambda, [&m](double x) { return m.eval(x); }};
  std::cout << "std::function:   ";
  for (auto const &f : fs)
    std::cout << " " << f(2.);
  std::cout << std::endl;

  // Every lambda has its own, unnamed, type
  auto l1 = [](double x) { return x; };
  auto l2 = [](double x) { return x; };
  std::cout << "same type? " << std::boolalpha << std::is_same_v<decltype(l1), decltype(l2)> << std::endl;
  std::cout << "sizeof: lambda(a) = " << sizeof(lambda) << ", lambda_ref = " << sizeof(lambda_ref)
            << ", std::function = " << sizeof(std::function<double(double)>) << std::endl;
  return 0;
}
