#include <iostream>
#include <memory>
#include <vector>
#include "utils.h"

int main(int argc, char** argv)
{
    type_fun func = [](double const& x){return x*x-2;};
    type_fun der_func = [](double const& x){return 2*x;};

    vector<shared_ptr<Root_finder> > root_method;

    int max_iter = 1e3;
    double rtol = 0.01;
    double stol = 1e-3;
    
    root_method.push_back(make_shared<Newton>(2.0, func, der_func, max_iter, rtol, stol));
    root_method.push_back(make_shared<Bisection>(3.1, 1, func, max_iter, rtol, stol));


    for (const auto& r : root_method)
    {
        cout << r->solve() << " " << r->getName() << endl;
    }

    return 0;
}
