/*
Sets and maps: what do they contain, and in which order?

compile with
  g++ -Wall -std=c++20 containers.cpp -o containers
and run with
  ./containers

Before running, try to predict the output of each point!
*/

#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

// Prints any container whose elements can be printed with <<: a function template
template <typename Container>
void print(const std::string &title, const Container &c) {
  std::cout << "  " << title << ":";
  for (const auto &x : c)
    std::cout << " " << x;
  std::cout << std::endl;
}

// A comparison object with a state: a and b are equivalent if they have the
// same remainder modulo mod, i.e. if neither a < b nor b < a
struct ModCompare {
  int mod;
  bool operator()(int a, int b) const { return a % mod < b % mod; }
};

int main() {
  const std::vector<int> v{5, 3, 8, 3, 1, 8, 9, 2};
  print("vector", v);

  std::cout << "1. sets: built from the range [v.begin(), v.end())" << std::endl;
  const std::set<int> s(v.begin(), v.end());
  print("set", s);
  const std::multiset<int> ms(v.begin(), v.end());
  print("multiset", ms);
  const std::set<int, std::greater<int>> sg(v.begin(), v.end());
  print("set<int, greater>", sg);
  const std::set<int, ModCompare> smod(v.begin(), v.end(), ModCompare{3});
  print("set<int, ModCompare{3}>", smod);
  const std::unordered_set<int> us(v.begin(), v.end());
  print("unordered_set", us);

  std::cout << "2. maps" << std::endl;
  std::map<std::string, std::string> phone{{"Marco", "022344789"}, {"Giorgio", "0119876656"}};
  const auto [it, inserted] = phone.insert({"Marco", "02339877"});
  std::cout << "  insert Marco: " << (inserted ? "inserted" : "not inserted") << ", Marco's number is "
            << it->second << std::endl;

  std::cout << "  size before: " << phone.size() << std::endl;
  if (phone["Maria"].empty())
    std::cout << "  Maria has no number" << std::endl;
  std::cout << "  size after: " << phone.size() << std::endl;
  for (const auto &[name, number] : phone)
    std::cout << "  " << name << ": '" << number << "'" << std::endl;

  std::cout << "3. searching without inserting" << std::endl;
  std::cout << "  contains Luca? " << (phone.contains("Luca") ? "yes" : "no") << std::endl;
  if (const auto g = phone.find("Giorgio"); g != phone.end())
    std::cout << "  found " << g->first << ": " << g->second << std::endl;
  try {
    std::cout << phone.at("Luca") << std::endl;
  } catch (const std::out_of_range &) {
    std::cout << "  at(\"Luca\") throws std::out_of_range" << std::endl;
  }
  std::cout << "  size: " << phone.size() << std::endl;

  // const std::map<std::string, std::string> &cphone = phone;
  // std::cout << cphone["Marco"] << std::endl; // try it
  return 0;
}
