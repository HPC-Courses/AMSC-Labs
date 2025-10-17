#ifndef JACOBIANFACTORY_HPP
#define JACOBIANFACTORY_HPP

#include <memory>
#include <type_traits>
#include "Jacobian.hpp"

//auto jac_full = make_jacobian<JacobianType::Full>(jacobian_fun);
//hand-made factory:
//			std::unique_ptr <JacobianBase> jac_full = std::make_unique <FullJacobian> (jacobian_fun);



//      auto jac_discrete = make_jacobian<JacobianType::Discrete>(system, h);
//	std::unique_ptr <JacobianBase> jac_discrete = std::make_unique <DiscreteJacobian> (system, h);


template <class JacobianType, class ... ARGS>
std::unique_ptr <JacobianBase>
make_jacobian (const ARGS & ... args)
{
	if constexpr (std::is_same_v <JacobianType, FullJacobian> )
		return std::make_unique <FullJacobian> (args...);
	else
		return std::make_unique <DiscreteJacobian> (args...);
}

/*

w/o the constexpr, eg
	if  (std::is_same_v <JacobianType, FullJacobian> )
g++ would have raised this compilation error

$ make
g++ -std=c++17 -O3 -Wall -pedantic -I. -I/usr/include/eigen3  -c -o main.o main.cpp
In file included from /usr/include/c++/13/memory:78,
                 from JacobianFactory.hpp:4,
                 from main.cpp:2:
/usr/include/c++/13/bits/unique_ptr.h: In instantiation of 'std::__detail::__unique_ptr_t<_Tp> std::make_unique(_Args&& ...) [with _Tp = DiscreteJacobian; _Args = {const main(int, char**)::<lambda(const VariableType&)>&}; __detail::__unique_ptr_t<_Tp> = __detail::__unique_ptr_t<DiscreteJacobian>]':
JacobianFactory.hpp:25:46:   required from 'std::unique_ptr<JacobianBase> make_jacobian(const ARGS& ...) [with JacobianType = FullJacobian; ARGS = {main(int, char**)::<lambda(const VariableType&)>}]'
main.cpp:47:50:   required from here
/usr/include/c++/13/bits/unique_ptr.h:1070:30: error: no matching function for call to 'DiscreteJacobian::DiscreteJacobian(const main(int, char**)::<lambda(const VariableType&)>&)'
 1070 |     { return unique_ptr<_Tp>(new _Tp(std::forward<_Args>(__args)...)); }
      |                              ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
In file included from main.cpp:1:
Jacobian.hpp:51:5: note: candidate: 'DiscreteJacobian::DiscreteJacobian(const NewtonTraits::JacobianFunctionType&, const double&)'
   51 |     DiscreteJacobian (const T::JacobianFunctionType & _system, const double & _h)
      |     ^~~~~~~~~~~~~~~~
Jacobian.hpp:51:5: note:   candidate expects 2 arguments, 1 provided
Jacobian.hpp:44:7: note: candidate: 'DiscreteJacobian::DiscreteJacobian(const DiscreteJacobian&)'
   44 | class DiscreteJacobian final: public JacobianBase
      |       ^~~~~~~~~~~~~~~~
Jacobian.hpp:44:7: note:   no known conversion for argument 1 from 'const main(int, char**)::<lambda(const VariableType&)>' to 'const DiscreteJacobian&'
Jacobian.hpp:44:7: note: candidate: 'DiscreteJacobian::DiscreteJacobian(DiscreteJacobian&&)'
Jacobian.hpp:44:7: note:   no known conversion for argument 1 from 'const main(int, char**)::<lambda(const VariableType&)>' to 'DiscreteJacobian&&'
/usr/include/c++/13/bits/unique_ptr.h: In instantiation of 'std::__detail::__unique_ptr_t<_Tp> std::make_unique(_Args&& ...) [with _Tp = FullJacobian; _Args = {const main(int, char**)::<lambda(const VariableType&)>&, const double&}; __detail::__unique_ptr_t<_Tp> = __detail::__unique_ptr_t<FullJacobian>]':
JacobianFactory.hpp:23:42:   required from 'std::unique_ptr<JacobianBase> make_jacobian(const ARGS& ...) [with JacobianType = DiscreteJacobian; ARGS = {main(int, char**)::<lambda(const VariableType&)>, double}]'
main.cpp:73:58:   required from here
/usr/include/c++/13/bits/unique_ptr.h:1070:30: error: no matching function for call to 'FullJacobian::FullJacobian(const main(int, char**)::<lambda(const VariableType&)>&, const double&)'
 1070 |     { return unique_ptr<_Tp>(new _Tp(std::forward<_Args>(__args)...)); }
      |                              ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Jacobian.hpp:35:17: note: candidate: 'FullJacobian::FullJacobian(const NewtonTraits::JacobianFunctionType&)'
   35 |                 FullJacobian (const T::JacobianFunctionType & _jac) : jac (_jac) {};
      |                 ^~~~~~~~~~~~
Jacobian.hpp:35:17: note:   candidate expects 1 argument, 2 provided
Jacobian.hpp:29:7: note: candidate: 'FullJacobian::FullJacobian(const FullJacobian&)'
   29 | class FullJacobian final: public JacobianBase
      |       ^~~~~~~~~~~~
Jacobian.hpp:29:7: note:   candidate expects 1 argument, 2 provided
Jacobian.hpp:29:7: note: candidate: 'FullJacobian::FullJacobian(FullJacobian&&)'
Jacobian.hpp:29:7: note:   candidate expects 1 argument, 2 provided
make: *** [<builtin>: main.o] Error 1

*/


#endif /* JACOBIANFACTORY_HPP */
