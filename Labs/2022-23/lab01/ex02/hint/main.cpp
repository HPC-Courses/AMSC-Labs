#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>

#include "horner.hpp"

int main() {
    std::cout << "Polynomial degree" << std::endl;
    std::cout << "=> ";
    /*get the polynomial degree in a unsigned int variable from standart input*/

    std::cout << "Coefficients are computed automatically" << std::endl;
    /*fill vector using 2sin(2k) formula for coefficients*/

    // some static parameters
    const double x_0 = 0.00;
    const double x_f = 1.00;
    const unsigned int n = 100000;
    const double h = (x_f - x_0) / (n - 1);

    // compute evaluation points an put them in a vector
    /*fill with code*/

    std::cout << "Computing " << n << " evaluations of polynomial"
        << " with standard formula" << std::endl;
    {
        // we use the namespace in a limited scope thanks to the curly brackets
        using namespace std::chrono; 
        // get current time using  'high_resolution_clock::now()'
        /*fill with code*/
        evaluate_poly(points, coeff, eval);
        /*fill with code*/
        // cast to milliseconds with 'duration_cast<milliseconds>'
        // convert to integer with '.count()' method
        const auto dt = /*fill with code*/;
        std::cout << "Elapsed: " << dt << " [ms]" << std::endl;
    }

    std::cout << "Computing " << n << " evaluations of polynomial"
        << " with horner formula" << std::endl;
    {
        using namespace std::chrono;
        /*get current time*/
        evaluate_poly(points, coeff, eval_horner);
        /*compute elapsed time as before*/
        
    }

    return 0;
}