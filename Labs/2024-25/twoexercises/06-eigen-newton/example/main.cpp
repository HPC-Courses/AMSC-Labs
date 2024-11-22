#include <Eigen/Dense>
#include <iostream>

int main(){
  // use eigen::vector for compability with eigen
  Eigen::VectorXd b(5);

  // stream operator to define the input
  b << 1.5, 3, 2.2, 0, 10;

  // overload of call operator for 
  b(0) = 0;
  b(2) = 2;
  // ...

  Eigen::MatrixXd A(5,5); // nrows, ncols

  // you can modify the columns with the keyword col:
  A.col(0) = b;

  for (size_t i=0; i < A.rows(); i++)
    for (size_t j=0; j < A.rows(); j++)
      A(i,j) = 1 / (i+j+1.0);

  std::cout << A*b << std::endl;

  std::cout << A.fullPivLu().solve(b) << std::endl;

}
