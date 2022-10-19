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

template <typename T>
bool eq(const T& lhs, const T& rhs) {
  if (lhs.size() != rhs.size()) {
    return false;
  }
  for (auto i = lhs.begin(), j = rhs.begin(); i != lhs.end() && j != rhs.end();
       ++i, ++j) {
    if (*i != *j) {
      return false;
    }
  }
  return true;
}

void print_bool(bool b) {
  std::cout << (b ? "PASSED" : "FAILED") << std::endl;
}

int main() {
  // --------------------------------------------------------------------------
  // ASSIGMENT 1
  // sort and remove the duplicates from the given vector v
  // use just std::set
  const std::vector<int> v = {7, 8, 9, 2, 0, 7, 6, 4, 1, 1, 0, 4, 5, 6, 7, 3};
  std::vector<int> sorted_and_unique;
  // WRITE HERE YOUR SOLUTION
  print_bool(eq(sorted_and_unique, {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}}));
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
  print_bool(
      eq(word_count,
         {{"a", 1},         {"and", 1},      {"at", 2},      {"behind", 1},
          {"different", 1}, {"dust", 1},     {"either", 1},  {"evening", 1},
          {"fear", 1},      {"from", 1},     {"handful", 1}, {"i", 2},
          {"in", 1},        {"meet", 1},     {"morning", 1}, {"of", 1},
          {"or", 1},        {"rising", 1},   {"shadow", 2},  {"show", 2},
          {"something", 1}, {"striding", 1}, {"to", 1},      {"will", 2},
          {"you", 4},       {"your", 2}}));
  // --------------------------------------------------------------------------
  // --------------------------------------------------------------------------
  // --------------------------------------------------------------------------
  // ASSIGMENT 3
  // elvaluate the functions specified in the vector funs at the point x=0.9
  // instert the results fun(0.9) into the vector fx
  // exploit the given unordered map to convert the string into an std::function
  // you will need to add elements to the given fmap
  const std::unordered_map<std::string, std::function<double(double)>> fmap =
  {
    {"cos", [](auto x){return std::cos(x); }},
    // ADD HERE OTHER NEEDED ELEMENTS
  };
  const std::vector<std::string> funs = {"tan", "sin", "exp"};
  std::vector<double> fx;
  // WRITE HERE YOUR SOLUTION
  print_bool(eq(fx, {{std::tan(0.9), std::sin(0.9), std::exp(0.9)}}));
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
  // WRITE HERE YOUR SOLUTION
  print_bool(eq(range11even_squares, {{0, 4, 16, 36, 64, 100}}));
  // --------------------------------------------------------------------------
  return 0;
}