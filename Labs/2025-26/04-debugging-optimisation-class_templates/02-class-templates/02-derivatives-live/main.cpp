#include "Derivatives.hpp"

#include <iostream>
//#include <functional> 
#include <cmath>

/*
template <unsigned N,
          typename F,
          typename T = double;
          typename DT = DifferenceType::FORWARD> 
class NthDerivative
*/

int main ()
{
  double h = 0.01;
  
  std::function <double (const double &)> f = [] (const double &x) {return std::exp(x);};
  
  std::cout << "The 4th derivative of e^x evaluated in 1 is "
            << NthDerivative <4, decltype (f) > {f, h} (1.)
            << std::endl;
  return 0;
}