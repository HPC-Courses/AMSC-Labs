#ifndef DERIVATIVES_HPP
#define DERIVATIVES_HPP

#include <type_traits>

// The two finite differences. They contain no data and no functions: they are only
// used as template arguments. Other is the type of the other difference: a trait
// (slide 10), a struct that collects type aliases.
struct Backward; // forward declaration: Forward needs this name

struct Forward {
  using Other = Backward;
};

struct Backward {
  using Other = Forward;
};

// The N-th derivative of the callable f: the first difference of the derivative of
// order N - 1, which is a NthDerivative too. This is a recursion, on the types.
// N is a non-type template parameter, T the type of the numbers, DT the difference.
template <unsigned N, typename F, typename T = double, typename DT = Forward>
class NthDerivative {
public:
  // The difference alternates at each order. DT::Other depends on a template
  // parameter, and it is a type: we must say so, with typename (slide 9)
  using Previous = NthDerivative<N - 1, F, T, typename DT::Other>;

  NthDerivative(const F &f, const T &h) : previous_{f, h}, h_{h} {}

  T operator()(const T &x) const {
    if constexpr (std::is_same_v<DT, Forward>)
      return (previous_(x + h_) - previous_(x)) / h_;
    else
      return (previous_(x) - previous_(x - h_)) / h_;
  }

private:
  Previous previous_;
  T h_;
};

// The end of the recursion: a partial specialization, with N = 0 and any F, T and DT.
// The derivative of order 0 is the function itself.
template <typename F, typename T, typename DT>
class NthDerivative<0, F, T, DT> {
public:
  NthDerivative(const F &f, const T &) : f_{f} {}

  T operator()(const T &x) const { return f_(x); }

private:
  F f_;
};

// We write N, and the compiler deduces F and T from the arguments
template <unsigned N, typename F, typename T>
auto make_derivative(const F &f, const T &h) {
  return NthDerivative<N, F, T>{f, h};
}

#endif /* DERIVATIVES_HPP */
