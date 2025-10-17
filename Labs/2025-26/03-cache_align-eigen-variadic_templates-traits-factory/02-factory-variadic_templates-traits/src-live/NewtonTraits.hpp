#ifndef NEWTONTRAITS_HPP
#define NEWTONTRAITS_HPP

#include <Eigen/Dense>
#include <functional>

class NewtonTraits
{
	public:
		using VariableType = Eigen::VectorXd;
		using JacobianMatrixType = Eigen::MatrixXd;

		using JacobianFunctionType = std::function < JacobianMatrixType (const VariableType &)  >;
		// y = f(x)
		using NonLinearSystemType = std::function < VariableType (const VariableType &) >;
};

#endif /* NEWTONTRAITS_HPP */
