#ifndef NEWTON_H
#define NEWTON_H

#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <vector>

class NewtonSolver {
public:
  NewtonSolver(
    // function f
    // derivative of f
    // max iterations 
    // residual tolerance, by default 2 machine epsilon
    // step tolerance, by default 2 machine epsilon
  )
    :
  /*initialize members*/ {
  }


// ================================================
// implement here the public method "solve" that recives as input x_0
// has the following pseudocode
// * reset the history of solutions x_n
// * while not reached max number of iterations
// ** compute residual at current x_iter
// ** if residual too small break
// ** compute derivative at current x_iter
// ** apply newton step
// ** if step is smaller than tolerance break

// ================================================
// define here the public "getters" for
// * the result
// * the last step size
// * the history of x_n (return a const reference)
// * the last residual
// * the number of iterations performed

// ================================================
// define here the private members, you need
// * the function f
// * the derivative of f
// * max number of iterations
// * the tolerance on the residual
// * the tolerance of the step size
// * the history of the iterations x_n
// * the last evaluation of f
// * the last evaluation of the derivative of f
// * the current residual
// * the current number of iterations
};

#endif /* NEWTON_H */