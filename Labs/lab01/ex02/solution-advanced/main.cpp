#include <iostream>
#include <vector>
#include <cmath>
#include <ranges>

#include "horner.hpp"

int main() {
    // parse parameter from file and print them
    const auto parameters = parse_parameters(get_file_contents("params.dat"));
    std::cout << "Parsed parameter are:" << std::endl;
    for (const auto& [key, value] : parameters)
        std::cout << "-- " << key << ": " << value << std::endl;

    // save parameters locally for ease of use (optional)
    const auto x0 = parameters.at("x_0");
    const auto xf = parameters.at("x_f");
    const auto n = static_cast<unsigned int>(parameters.at("n_points"));
    const auto degree = static_cast<unsigned int>(parameters.at("degree"));
    const auto h = (xf - x0) / (n - 1);

    // construct vector of points and coefficients with ranges
    const auto x_range = std::ranges::views::iota(0u, n) |
        std::ranges::views::transform([=](auto n)
            { return x0 + n * h; });
    const std::vector<double> points(x_range.begin(), x_range.end());

    const auto coeff_range = std::ranges::views::iota(0u, degree + 1) |
        std::ranges::views::transform([=](auto n)
            { return 2 * std::sin(2.0 * n); });
    const std::vector<double> coeff(coeff_range.begin(), coeff_range.end());

    // define the tests we are going to perform and loop over them
    const std::unordered_map<std::string, eval_method_t> TESTS = {
        {"standard std::pow", eval_std},
        {"standard integer pow", eval_pow_integer},
        {"standard pow by squaring", eval_squaring},
        {"standard pow branchless squaring", eval_branchless},
        {"horner", eval_horner},
    };
    for (const auto parallel_policy : { true, false }) {
        std::cout << "--------------------------------------" << std::endl;
        std::cout << "  parallel execution: " << (parallel_policy ? "ON" : "OFF") << std::endl;
        std::cout << "--------------------------------------" << std::endl;
        for (const auto& [test_name, test_func] : TESTS) {
            std::cout << "Computing " << n << " evaluations of polynomial"
                << " with " << test_name << " formula" << std::endl;
            const auto dt = timeit([&]()
                { evaluate_poly(points, coeff, test_func, parallel_policy); });
            std::cout << "Elapsed: " << dt << " [ms]" << std::endl;
        }
    }


    return 0;
}