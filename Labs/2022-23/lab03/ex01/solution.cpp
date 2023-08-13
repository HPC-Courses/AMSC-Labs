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
  // 1
  // sort and remove the duplicates from the given vector v
  // use std::set
  const std::vector<int> v = {7, 8, 9, 2, 0, 7, 6, 4, 1, 1, 0, 4, 5, 6, 7, 3};
  std::set<int> s(v.begin(), v.end());
  std::vector<int> sorted_and_unique(s.begin(), s.end());
  // check your solution
  print_bool(eq(sorted_and_unique, {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}}));
  // --------------------------------------------------------------------------
  // 2
  // given a tokenized sentence in a vector, compute
  // how many times each word appears in a std::map
  const std::vector<std::string> words = {
      "and",    "i",      "will", "show",   "you",     "something", "different",
      "from",   "either", "your", "shadow", "at",      "morning",   "striding",
      "behind", "you",    "or",   "your",   "shadow",  "at",        "evening",
      "rising", "to",     "meet", "you",    "i",       "will",      "show",
      "you",    "fear",   "in",   "a",      "handful", "of",        "dust"};
  std::map<std::string, size_t> word_count;
  for (const auto& w : words) {
    word_count[w]++;
  }
  // check your solution
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
  // 3
  // compute the value of the functions in the vector f at the point x=0.9
  // exploit the given unordered map
  const std::unordered_map<std::string, std::function<double(double)>> fmap =
  {
    {"sin", [](auto x){return std::sin(x); }},
    {"cos", [](auto x){return std::cos(x); }},
    {"exp", [](auto x){return std::exp(x); }},
    {"sqrt", [](auto x){return std::sqrt(x); }},
    {"tan", [](auto x){return std::tan(x); }},
  };
  const std::vector<std::string> funs = {"tan", "sin", "exp"};
  std::vector<double> fx;
  for(const auto &fun : funs) {
    fx.push_back(fmap.at(fun)(0.9));
  }
  // check your solution
  print_bool(eq(fx, {{std::tan(0.9), std::sin(0.9), std::exp(0.9)}}));
  // --------------------------------------------------------------------------
  // 4
  // compute the square of each even number between 0 and 11 starting from
  // the vector range11
  std::vector<int> range11(11);
  std::iota(range11.begin(), range11.end(), 0);
  std::vector<int> range11even_squares(6);
  std::copy_if(range11.begin(), range11.end(), range11even_squares.begin(), [](auto x) {return (x % 2) == 0;});
  std::for_each(range11even_squares.begin(), range11even_squares.end(), [](auto &x){x = x * x;});
  // check your solution
  print_bool(eq(range11even_squares, {{0, 4, 16, 36, 64, 100}}));
  // --------------------------------------------------------------------------
  return 0;
}