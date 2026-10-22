/*
Six small exercises on the containers and the algorithms of the standard library.
Solution.

compile with
  g++ -Wall -std=c++20 solution.cpp -o solution
and run with
  ./solution
*/

#include <algorithm>
#include <cmath>
#include <functional>
#include <iterator>
#include <map>
#include <numeric>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "check.hpp"

using WordCount = std::map<std::string, std::size_t>;

// Returns how many times word appears in word_count, or an empty optional if it
// never appears. word_count[word] does not compile: [] inserts the missing keys,
// so it cannot be called on a const map
std::optional<std::size_t> count_of(const WordCount &word_count, const std::string &word) {
  const auto it = word_count.find(word);
  if (it == word_count.end())
    return std::nullopt; // empty
  return it->second;
}

int main() {
  // ---------------------------------------------------------------------------
  // ASSIGNMENT 1
  // Sort v and remove the duplicates, using only a std::set
  const std::vector<int> v = {7, 8, 9, 2, 0, 7, 6, 4, 1, 1, 0, 4, 5, 6, 7, 3};
  // a set keeps its elements sorted, and does not store the same element twice
  const std::set<int> s(v.begin(), v.end());
  const std::vector<int> sorted_and_unique(s.begin(), s.end());
  check1(sorted_and_unique);

  // ---------------------------------------------------------------------------
  // ASSIGNMENT 2
  // Count how many times each word appears in words
  const std::vector<std::string> words = {
      "and", "i", "will", "show", "you", "something", "different", "from",
      "either", "your", "shadow", "at", "morning", "striding", "behind", "you",
      "or", "your", "shadow", "at", "evening", "rising", "to", "meet", "you",
      "i", "will", "show", "you", "fear", "in", "a", "handful", "of", "dust"};
  WordCount word_count;
  // If w is not in the map yet, [] inserts it with a value-initialised count, i.e. 0
  for (const auto &w : words)
    ++word_count[w];
  check2(word_count);

  // ---------------------------------------------------------------------------
  // ASSIGNMENT 3
  // Evaluate the functions named in funs at x = 0.9, and store the results, in
  // the same order, in fx. Do not hard-code tan(0.9) etc.: add the missing
  // functions to fmap, then use it to turn each name into a function
  const std::unordered_map<std::string, std::function<double(double)>> fmap = {
      // {"cos", std::cos} does not compile: std::cos is overloaded (float, double,
      // ...), and the compiler does not know which one you mean. A lambda solves it
      {"cos", [](double x) { return std::cos(x); }},
      {"sin", [](double x) { return std::sin(x); }},
      {"tan", [](double x) { return std::tan(x); }},
      {"exp", [](double x) { return std::exp(x); }},
  };
  const std::vector<std::string> funs = {"tan", "sin", "exp"};
  std::vector<double> fx;
  // at() and not []: fmap is const (and a misspelled name throws, instead of
  // inserting an empty function)
  std::transform(funs.begin(), funs.end(), std::back_inserter(fx),
                 [&fmap](const std::string &name) { return fmap.at(name)(0.9); });
  // The same with a loop:
  // for (const auto &name : funs)
  //   fx.push_back(fmap.at(name)(0.9));
  check3(fx);

  // ---------------------------------------------------------------------------
  // ASSIGNMENT 4
  // Compute the squares of the even numbers in range, using
  //  - std::copy_if with std::back_inserter, to copy the even numbers into even_squares
  //  - std::for_each, to square them in place
  std::vector<int> range(11);
  std::iota(range.begin(), range.end(), 0); // 0, 1, ..., 10
  std::vector<int> even_squares;
  // back_inserter: every element copied by copy_if is added with push_back
  std::copy_if(range.begin(), range.end(), std::back_inserter(even_squares),
               [](int x) { return x % 2 == 0; });
  std::for_each(even_squares.begin(), even_squares.end(), [](int &x) { x *= x; });
  check4(even_squares);

  // ---------------------------------------------------------------------------
  // ASSIGNMENT 5
  // Implement count_of, at the top of this file
  check5(count_of(word_count, "shadow"), count_of(word_count, "luca"));

  // ---------------------------------------------------------------------------
  // ASSIGNMENT 6
  // The three most frequent words, with their counts, from the most frequent.
  // Words with the same count are in alphabetical order.
  // Hint: copy word_count into a std::vector of pairs, then sort it.
  // the elements of a map are pairs (key, value): they can be copied into a vector
  std::vector<std::pair<std::string, std::size_t>> top3(word_count.begin(), word_count.end());
  // the vector is in alphabetical order, as the map: stable_sort keeps that order
  // among the words with the same count
  std::stable_sort(top3.begin(), top3.end(), [](const auto &a, const auto &b) { return a.second > b.second; });
  top3.resize(3);
  check6(top3);

  return 0;
}
