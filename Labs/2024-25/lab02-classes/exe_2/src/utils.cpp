#include "utils.h"
#include <cmath>
#include <iostream>

int
Root_finder::getIter() {return iter;}

double
Root_finder::getRes() {return residual;}

vector<double>
Root_finder::getXs() {return xs;}

Newton::Newton(const double initial_guess, type_fun func, type_fun der_func, int max_iter, double rtol, double stol) : initial_guess(initial_guess), Root_finder(func, max_iter, rtol, stol), der_fun(der_fun) {}

Bisection::Bisection(const double a, const double b, type_fun func, int max_iter, double rtol, double stol) : a(a), b(b), Root_finder(func, max_iter, rtol, stol) {}

