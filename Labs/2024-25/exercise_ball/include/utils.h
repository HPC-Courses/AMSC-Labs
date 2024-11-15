#ifndef UTILS_HH
#define UTILS_HH


#include <iostream>
#include <cmath>
#include <functional>
#include <optional>
#include <vector>
using namespace std;


// in the .h file you declare the functions, classes, ..., while in the .cpp you define them

using type_fun  = std::function<double(double const&)>;
using type_fun2  = std::function<double()>;

class Root_finder{
    public:
    Root_finder(type_fun, int, double, double);
    virtual ~Root_finder() = default; //Always make virtual the destructor when working with class inheritance 
    virtual double solve() = 0; //Pure virtual class
    constexpr virtual const char *getName() = 0; //Make also this virtual to resolve at compile time the name
    int getIter();
    double getRes();
    vector<double> getXs();
    double current_root_estimate;
    type_fun function_root;
    int max_iter;
    double rtol;
    double stol;
    int iter = 0;
    double residual;
    double delta_x;
    vector<double> xs;
};


class Newton: public Root_finder{
    private:
    double initial_guess;
    type_fun der_fun;
    double step_size;
    bool is_Newton;

    public:
    Newton(const double, type_fun, int, double, double, optional<type_fun> der_func_in = nullopt, double step_size = 1e-5);
    virtual ~Newton() override = default;
    double solve() override 
    { 
        // Implement here the method!
        current_root_estimate = initial_guess;
        do 
        {
            delta_x = function_root(current_root_estimate)/der_fun(current_root_estimate);
            current_root_estimate -= delta_x; 
            xs.push_back(current_root_estimate);
            residual = std::abs(function_root(current_root_estimate));
        } while (++iter<max_iter && std::abs(delta_x)>stol && residual>rtol);
        return(current_root_estimate); 
    };
    constexpr virtual const char *getName() override { return (is_Newton ? "Newton" : "Secant"); };
};


class Bisection: public Root_finder{
    private:
    double a,b;

    public:
    Bisection(const double, const double, type_fun, int, double, double);
    virtual ~Bisection() override = default;
    double solve() override 
    { 
        // Implement here the method!
        if (function_root(a)*function_root(b) > 0)
        {
            cout << "Bisection method cannot find the zero of this function" << endl;
            exit(1); 
        }
        do
        {
            current_root_estimate = .5*(a + b);

            a = function_root(current_root_estimate)*function_root(b) < 0 ? current_root_estimate : a;
            b = function_root(current_root_estimate)*function_root(a) < 0 ? current_root_estimate : b;

            delta_x = abs(b - a); // estimation of the iterate difference
            xs.push_back(current_root_estimate);
            residual = std::abs(function_root(current_root_estimate));
        } while (++iter<max_iter && std::abs(delta_x)>stol && residual>rtol);
        return(current_root_estimate); 
    };
    constexpr virtual const char *getName() override { return "Bisection"; };
};

#endif
