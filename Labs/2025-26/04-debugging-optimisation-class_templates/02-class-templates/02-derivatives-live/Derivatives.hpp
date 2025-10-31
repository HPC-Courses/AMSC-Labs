#ifndef DERIVATIVES
#define DERIVATIVES

#include <functional>
#include <type_traits>

namespace DifferenceType
{
  struct FORWARD;
  
  struct BACKWARD
  {
    using otherType = FORWARD;
  };
  
  struct FORWARD
  {
    using otherType = BACKWARD;
  };
}

template <unsigned N,
          typename F,
          typename T = double,
          typename DT = DifferenceType::FORWARD> 
class NthDerivative
{
  //recursion
  public:
      using PreviousDerivative = NthDerivative <N-1, F, T, 
      typename DT::otherType>;
      //DT>;
      
      NthDerivative (const F & f, const T& h_) : pDerivative {f, h_}, h(h_) {}; //don't write only pDerivative (f);
      
      T operator () (const T& x) const
      {
        if constexpr (std::is_same <DifferenceType::FORWARD, DT> :: value)
           return ( pDerivative(x+h) - pDerivative (x) )/h;
        else
          return ( pDerivative(x) - pDerivative (x-h) )/h;
      
      }
      
  private:
   PreviousDerivative pDerivative;
   T h;
};


template <//unsigned N,
          typename F,
          typename T,
          typename DT> 
class NthDerivative <1u, F, T, DT>
{
  //recursion
  public:      
      NthDerivative (const F & f, const T& h_) : pDerivative {f}, h(h_) {};
      
      T operator () (const T& x) const
      {
        if (std::is_same <DifferenceType::FORWARD, DT> :: value)
           return ( pDerivative(x+h) - pDerivative (x) )/h; //pDerivative or f
        else
          return ( pDerivative(x) - pDerivative (x-h) )/h;
      
      }
      
  private:
   F pDerivative;
   T h;
};


#endif
