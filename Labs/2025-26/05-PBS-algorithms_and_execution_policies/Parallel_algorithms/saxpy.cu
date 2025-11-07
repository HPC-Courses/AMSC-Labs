#include <ctime>
#include <iostream>
 
static clock_t c_start, c_diff;
static double c_msec;
#define tic() c_start = clock ();
#define toc(x) c_diff = clock () - c_start;                   \
  c_msec = (double)c_diff * 1000 / (double)CLOCKS_PER_SEC;    \
  std::cout << x << c_msec << std::endl;

#include <thrust/transform.h>
#include <thrust/device_vector.h>
#include <thrust/execution_policy.h>
#include <thrust/sequence.h>

using real_t = float;

struct saxpy_functor_t
{
   const real_t a;
   saxpy_functor_t (real_t _a) : a(_a) {}

   __host__ __device__
   real_t operator()(const real_t& x, const real_t& y) const {
     return a * x + y;
   }
};


int main ()
{
   thrust::device_vector <real_t> x (1<<30), y (1<<30);
   thrust::sequence (x.begin (), x.end ());
   thrust::sequence (y.begin (), y.end ());
   real_t a = 2.0;

   //saxpy
   tic ();
   thrust::transform (thrust::device,
                               x.begin (), x.end (),
                               y.begin (), y.begin (),
                               saxpy_functor_t (a));
   toc ("saxpy execution lasts [ms] = ");

   return 0;
}
