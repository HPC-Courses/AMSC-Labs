/*
Six small exercises on the containers and the algorithms of the standard library.
Write your solution where you find TODO: check.hpp tells you if it is correct.

compile with
  g++ -Wall -std=c++20 assignment.cpp -o assignment
and run with
  ./assignment
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

// TODO (assignment 5): return how many times word appears in word_count, or an
//      empty optional if it never appears. word_count is const: can you use []?
std::optional<std::size_t> count_of(const WordCount &word_count, const std::string &word) {
  (void)word_count, (void)word; // remove this line when you use them
  return {};
}

int main() {
  // ---------------------------------------------------------------------------
  // ASSIGNMENT 1
  // Sort v and remove the duplicates, using only a std::set
  const std::vector<int> v = {7, 8, 9, 2, 0, 7, 6, 4, 1, 1, 0, 4, 5, 6, 7, 3};
  std::vector<int> sorted_and_unique;
  // TODO
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
  // TODO
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
      // TODO: add the functions you need
  };
  const std::vector<std::string> funs = {"tan", "sin", "exp"};
  std::vector<double> fx;
  // TODO
  check3(fx);

  // ---------------------------------------------------------------------------
  // ASSIGNMENT 4
  // Compute the squares of the even numbers in range, using
  //  - std::copy_if with std::back_inserter, to copy the even numbers into even_squares
  //  - std::for_each, to square them in place
  std::vector<int> range(11);
  std::iota(range.begin(), range.end(), 0); // 0, 1, ..., 10
  std::vector<int> even_squares;
  // TODO
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
  std::vector<std::pair<std::string, std::size_t>> top3;
  // TODO
  check6(top3);

  return 0;
}
