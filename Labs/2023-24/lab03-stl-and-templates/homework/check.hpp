#ifndef HH_UTILS_HH
#define HH_UTILS_HH

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

void print_passed(bool b) {
  std::cout << (b ? "PASSED" : "FAILED") << std::endl;
}

void check1(const std::vector<int> &sorted_and_unique) {
    print_passed(eq(sorted_and_unique, {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}}));
}

void check2(const std::map<std::string, size_t> &word_count) {
    print_passed(
      eq(word_count,
         {{"a", 1},         {"and", 1},      {"at", 2},      {"behind", 1},
          {"different", 1}, {"dust", 1},     {"either", 1},  {"evening", 1},
          {"fear", 1},      {"from", 1},     {"handful", 1}, {"i", 2},
          {"in", 1},        {"meet", 1},     {"morning", 1}, {"of", 1},
          {"or", 1},        {"rising", 1},   {"shadow", 2},  {"show", 2},
          {"something", 1}, {"striding", 1}, {"to", 1},      {"will", 2},
          {"you", 4},       {"your", 2}}));
}

void check3(const std::vector<double> &fx_at_09) {
    print_passed(eq(fx_at_09, {{std::tan(0.9), std::sin(0.9), std::exp(0.9)}}));
}

void check4(const std::vector<int> &range11even_squares) {
    print_passed(eq(range11even_squares, {{0, 4, 16, 36, 64, 100}}));
}

#endif