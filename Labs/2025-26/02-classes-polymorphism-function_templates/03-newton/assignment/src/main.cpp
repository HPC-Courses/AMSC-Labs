#include <iostream>
#include <memory>
#include <vector>
#include "utils.h"

static const double step_size = 1e-6;
static type_fun func = [](double const& x){return x*x-2.;};
static type_fun der_func = [](double const& x){return 2.*x;};

int main(int argc, char** argv)
{

    vector<shared_ptr<Root_finder> > root_method;
    root_method.reserve(3);

    int max_iter = 1e3;
    double rtol = 1e-5;
    double stol = 1e-5;
    
    root_method.push_back(make_shared<Newton>(2.0, func, max_iter, rtol, stol, nullopt, step_size)); // Secant method 
    root_method.push_back(make_shared<Newton>(2.0, func, max_iter, rtol, stol, der_func)); // Pure Newton method
    root_method.push_back(make_shared<Bisection>(0, 2, func, max_iter, rtol, stol)); // Bisection 

    for (const auto& r : root_method)
    {
        cout << r->solve() << " " << r->getName() << " " << r->getIter() << endl;
    }

    return 0;
}
