#ifndef JACOBIAN_HPP
#define JACOBIAN_HPP

#include "NewtonTraits.hpp"

/// The base class for representing the Jacobian.
///
/// It implements the basic methods for the application of the
/// Jacobian, which in fact may be a "pseudo-Jacobian" for
/// quasi-Newton methods or even Picard iterations.
class JacobianBase
{
public:
  /// Short-hand alias.
  using T = NewtonTraits;

  /// Default constructor.
  JacobianBase() = default;

  /// Solve J(x) * delta_x = res.
  virtual T::VariableType
  solve(const T::VariableType &x, const T::VariableType &res) const = 0;

  /// Destructor. Needed since this is a pure virtual class.
  virtual ~JacobianBase() = default;
};


class FullJacobian final: public JacobianBase
{
	private:
		T::JacobianFunctionType jac;

	public:
		FullJacobian (const T::JacobianFunctionType & _jac) : jac (_jac) {};

	 /// Solve J(x) * delta_x = res.
   virtual T::VariableType
   solve(const T::VariableType &x, const T::VariableType &res) const override;

};


class DiscreteJacobian final: public JacobianBase
{
  private:
    T::NonLinearSystemType system;
    const double h;

  public:
    DiscreteJacobian (const T::JacobianFunctionType & _system, const double & _h)
     : system (_system), h(_h) {};

   /// Solve J(x) * delta_x = res.
   virtual T::VariableType
   solve(const T::VariableType &x, const T::VariableType &res) const override;

};




#endif /* JACOBIAN_HPP */
