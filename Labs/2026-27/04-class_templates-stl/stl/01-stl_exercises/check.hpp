#ifndef CHECK_HPP
#define CHECK_HPP

// Compares your results with the expected ones. Do not modify this file.

#include <cmath>
#include <cstddef>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

// Containers, pairs and optionals all have operator==
template <typename T>
void check(int assignment, const T &result, const T &expected) {
  std::cout << "Assignment " << assignment << ": " << (result == expected ? "PASSED" : "FAILED") << std::endl;
}

inline void check1(const std::vector<int> &sorted_and_unique) {
  check(1, sorted_and_unique, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
}

inline void check2(const std::map<std::string, std::size_t> &word_count) {
  check(2, word_count,
        {{"a", 1},         {"and", 1},      {"at", 2},      {"behind", 1},
         {"different", 1}, {"dust", 1},     {"either", 1},  {"evening", 1},
         {"fear", 1},      {"from", 1},     {"handful", 1}, {"i", 2},
         {"in", 1},        {"meet", 1},     {"morning", 1}, {"of", 1},
         {"or", 1},        {"rising", 1},   {"shadow", 2},  {"show", 2},
         {"something", 1}, {"striding", 1}, {"to", 1},      {"will", 2},
         {"you", 4},       {"your", 2}});
}

inline void check3(const std::vector<double> &fx) {
  check(3, fx, {std::tan(0.9), std::sin(0.9), std::exp(0.9)});
}

inline void check4(const std::vector<int> &even_squares) {
  check(4, even_squares, {0, 4, 16, 36, 64, 100});
}

inline void check5(const std::optional<std::size_t> &shadow, const std::optional<std::size_t> &luca) {
  using Count = std::optional<std::size_t>;
  check(5, std::pair{shadow, luca}, std::pair{Count{2}, Count{}}); // Count{} is empty
}

inline void check6(const std::vector<std::pair<std::string, std::size_t>> &top3) {
  check(6, top3, {{"you", 4}, {"at", 2}, {"i", 2}});
}

#endif /* CHECK_HPP */
