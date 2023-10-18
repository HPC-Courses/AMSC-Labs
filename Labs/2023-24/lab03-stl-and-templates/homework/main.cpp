#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#include <numeric>

#include "check.hpp"


int main() {
  // --------------------------------------------------------------------------
  // ASSIGMENT 1
  // sort and remove the duplicates from the given vector v
  // use just std::set
  const std::vector<int> v = {7, 8, 9, 2, 0, 7, 6, 4, 1, 1, 0, 4, 5, 6, 7, 3};
  std::vector<int> sorted_and_unique;
  // WRITE HERE YOUR SOLUTION
  check1(sorted_and_unique);


  // --------------------------------------------------------------------------
  // --------------------------------------------------------------------------
  // --------------------------------------------------------------------------
  // ASSIGMENT 2
  // given a tokenized sentence in the vector words, compute
  // how many times each word appears, write the result in a std::map
  const std::vector<std::string> words = {
      "and",    "i",      "will", "show",   "you",     "something", "different",
      "from",   "either", "your", "shadow", "at",      "morning",   "striding",
      "behind", "you",    "or",   "your",   "shadow",  "at",        "evening",
      "rising", "to",     "meet", "you",    "i",       "will",      "show",
      "you",    "fear",   "in",   "a",      "handful", "of",        "dust"};
  std::map<std::string, size_t> word_count;
  // WRITE HERE YOUR SOLUTION
  check2(word_count);


  // --------------------------------------------------------------------------
  // --------------------------------------------------------------------------
  // --------------------------------------------------------------------------
  // ASSIGMENT 3
  // elvaluate the functions specified in the vector `funs` at the point x=0.9
  // instert the results tan(0.9), sin(0.9), exp(0.9) into the vector fx.
  // Do NOT hard-code the solution but
  // exploit the given unordered map to convert the string into an std::function
  // then loop over the funs vector and insert the values in fx_at_09
  // You will need to add elements to the given fmap
  const std::unordered_map<std::string, std::function<double(double)>> fmap =
  {
    // you cannot do {"sin", std::sin}
    // because std::sin is an overloaded function: 
    // you must disambiguate which std::sin overload you mean
    // `(double(*)(double))&std::sin` is a solution but is less safe and flexible than a lambda
    {"cos", [](auto x){return std::cos(x); }},
    // ADD HERE OTHER NEEDED ELEMENTS
  };
  const std::vector<std::string> funs = {"tan", "sin", "exp"};
  std::vector<double> fx_at_09;
  // WRITE HERE YOUR SOLUTION
  check3(fx_at_09);


  // --------------------------------------------------------------------------
  // --------------------------------------------------------------------------
  // --------------------------------------------------------------------------
  // ASSIGMENT 4
  // compute the square of each even number between 0 and 11 starting from
  // the vector range10, employ the STL functions:
  // * copy_if
  // * for_each
  std::vector<int> range11(11);
  std::iota(range11.begin(), range11.end(), 0);
  std::vector<int> range11even_squares;
  // WRITE HERE YOUR SOLUTION
  check4(range11even_squares);


  return 0;
}