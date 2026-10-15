#include "dataset.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

void print(const Dataset &d) {
  std::cout << "  " << d.name() << ", " << d.size() << " values, mean " << d.mean() << std::endl;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <datasets file>" << std::endl;
    return 1;
  }
  std::ifstream file{argv[1]};
  if (!file) {
    std::cerr << "Cannot open " << argv[1] << std::endl;
    return 1;
  }

  // A vector of objects, not of pointers: the vector owns the datasets
  std::vector<Dataset> all;
  std::string line;
  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#')
      continue;
    std::istringstream iss{line};
    try {
      all.push_back(read_dataset(iss));
    } catch (const std::invalid_argument &e) {
      std::cerr << "Skipping invalid line: " << e.what() << std::endl;
    }
  }

  std::cout << "datasets:" << std::endl;
  for (const auto &d : all)
    print(d);

  // strain is in mm/m: convert it to m/m
  for (auto &d : all)
    if (d.name() == "strain") {
      d.scale(1e-3);
      std::cout << "strain in m/m:" << std::endl;
      print(d);
    }

  // Try to break it:
  // all[0].values_.clear();            // equivalent to all.front().values_.clear();
  //                                    // values_ is private
  // Dataset const c{"c", {1., 2.}};
  // c.scale(2.);                       // scale() is not const
  // Dataset e{"e", {}};                // compiles: what happens at run time?
  return 0;
}
