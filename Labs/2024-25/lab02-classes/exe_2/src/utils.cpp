#include "utils.h"
#include <cmath>
#include <iostream>

int
Root_finder::getIter() {return iter;}

double
Root_finder::getRes() {return residual;}

vector<double>
Root_finder::getXs() {return xs;}

Root_finder::Root_finder(type_fun func, int max_iter, double rtol, double stol) : function_root(func), max_iter(max_iter), rtol(rtol), stol(stol) 
{
    // allocate here the size!, it's important
    xs.reserve(max_iter);
}

Newton::Newton(const double initial_guess, type_fun func, int max_iter, double rtol, double stol, optional<type_fun> der_func_in, double step_size) : initial_guess(initial_guess), Root_finder(func, max_iter, rtol, stol), step_size(step_size)
{
    xs.push_back(initial_guess);
    der_fun = der_func_in ? *der_func_in : [this](double const& x){return ((this->function_root(x+this->step_size)-this->function_root(x))/this->step_size);};
    is_Newton = der_func_in ? true : false;
}

Bisection::Bisection(double a, double b, type_fun func, int max_iter, double rtol, double stol) : a(a), b(b), Root_finder(func, max_iter, rtol, stol) {}

