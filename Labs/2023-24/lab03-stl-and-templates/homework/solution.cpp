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
  std::set<int> s(v.begin(), v.end());
  std::vector<int> sorted_and_unique(s.begin(), s.end());
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
  for (const auto& w : words) {
    // the trick here is that as soon as you access the map with the [] operator, 
    // if the key doesn't exist it gets added. The int gets "value initialization" 
    // invoked - so it will get a value of 0.
    word_count[w]++;
  }
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
    {"sin", [](auto x){return std::sin(x); }},
    {"cos", [](auto x){return std::cos(x); }},
    {"exp", [](auto x){return std::exp(x); }},
    {"sqrt", [](auto x){return std::sqrt(x); }},
    {"tan", [](auto x){return std::tan(x); }},
  };
  const std::vector<std::string> funs = {"tan", "sin", "exp"};
  std::vector<double> fx_at_09;
  std::transform(
    funs.begin(), 
    funs.end(), 
    std::back_inserter(fx_at_09), 
    [&fmap](const std::string &s){ return fmap.at(s)(0.9); }
  );
  // the same in the "classical" way
  // for(const auto &fun : funs) {
  //   fx_at_09.push_back(fmap.at(fun)(0.9));
  // }
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
  std::vector<int> range11even_squares(6);
  std::copy_if(range11.begin(), range11.end(), range11even_squares.begin(), [](auto x) {return (x % 2) == 0;});
  std::for_each(range11even_squares.begin(), range11even_squares.end(), [](auto &x){x = x * x;});
  check4(range11even_squares);

  
  return 0;
}