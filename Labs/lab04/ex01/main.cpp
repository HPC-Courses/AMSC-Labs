#include <Eigen/Eigen>
#include <iostream>

#include "mylib.hpp"

using Eigen::MatrixXd;

int main(int argc, char** argv) {
  MatrixXd m = Eigen::MatrixXd::Random(2, 2);
  std::cout << "A 2x2 random matrix" << std::endl;
  std::cout << m << std::endl;

  mylib::my_awesome_function_1();
  return 0;
}