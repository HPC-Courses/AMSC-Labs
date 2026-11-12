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
  // TODO (Step 3): the difference must alternate at each order
  using Previous = NthDerivative<N - 1, F, T, DT>;

  NthDerivative(const F &f, const T &h) : previous_{f, h}, h_{h} {}

  // TODO (Step 2): (previous(x + h) - previous(x)) / h if DT is Forward,
  //                (previous(x) - previous(x - h)) / h if DT is Backward
  T operator()(const T &x) const {
    (void)x; // remove this line when you use it
    return T{};
  }

private:
  Previous previous_;
  T h_;
};

// TODO (Step 1): the end of the recursion, a partial specialization with N = 0.
// The derivative of order 0 is the function itself.

// TODO (Step 4): make_derivative<N>(f, h), which returns a NthDerivative<N, F, T>

#endif /* DERIVATIVES_HPP */
