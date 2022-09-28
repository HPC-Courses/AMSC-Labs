#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>

#include "horner.hpp"

int main() {
    unsigned int degree;
    std::cout << "Polynomial degree" << std::endl;
    std::cout << "=> ";
    std::cin >> degree;

    std::vector<double> coeff(degree + 1);
    std::cout << "Coefficients are computed automatically" << std::endl;
    for (unsigned int i = 0; i <= degree; ++i)
        coeff[i] = 2 * std::sin(2.0 * i);

    const double x_0 = 0.00;
    const double x_f = 1.00;
    const unsigned int n = 100000;
    const double h = (x_f - x_0) / (n - 1);

    std::vector<double> points(n + 1);
    for (unsigned int i = 0; i <= n; ++i)
        points[i] = x_0 + i * h;

    std::cout << "Computing " << n << " evaluations of polynomial"
        << " with standard formula" << std::endl;
    {
        using namespace std::chrono;
        const auto t0 = high_resolution_clock::now();
        evaluate_poly(points, coeff, eval);
        const auto t1 = high_resolution_clock::now();
        const auto dt = duration_cast<milliseconds>(t1 - t0).count();
        std::cout << "Elapsed: " << dt << " [ms]" << std::endl;
    }

    std::cout << "Computing " << n << " evaluations of polynomial"
        << " with horner formula" << std::endl;
    {
        using namespace std::chrono;
        const auto t0 = high_resolution_clock::now();
        evaluate_poly(points, coeff, eval_horner);
        const auto t1 = high_resolution_clock::now();
        const auto dt = duration_cast<milliseconds>(t1 - t0).count();
        std::cout << "Elapsed: " << dt << " [ms]" << std::endl;
    }

    return 0;
}