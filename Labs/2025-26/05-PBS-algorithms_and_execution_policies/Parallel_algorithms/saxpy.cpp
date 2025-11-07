#include <algorithm>
#include <iostream>
#include <vector>
#ifdef USE_BOOST
  #include <boost/iterator/counting_iterator.hpp>
#else
  #include <ranges>
#endif
#include <execution>

using idx_t = size_t;
using real_t = float;

int main()
{
  std::vector <real_t> x{0,1,2,3,4}, y{0,1,2,3,4};
  real_t a (5.);
  
  auto saxpy = [x = x.data (), y = y.data (), a] 
               (idx_t i)
               { y[i] += a * x[i];};
  
  #ifdef USE_BOOST
    boost::counting_iterator <idx_t> first(0), last(x.size ());
    std::for_each (std::execution::par_unseq, first, last, saxpy);
  #else
    auto indices = std::views::iota (static_cast<std::vector<real_t>::size_type> (0),  x.size ());
    std::for_each (std::execution::par_unseq, indices.begin (), indices.end (), saxpy);        
  #endif
  
  for (idx_t i=0; i<x.size(); ++i)
    std::cout << 5 << "x" << x[i] << "+" << x[i] << "=" << y[i] << std::endl;
    
  return 0;
 
}