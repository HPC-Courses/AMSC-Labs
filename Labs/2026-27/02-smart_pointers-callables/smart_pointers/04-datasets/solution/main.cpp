#include "datasets.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using DatasetVector = std::vector<std::unique_ptr<Dataset>>;

// Only reads the datasets: passed by const reference, nobody takes ownership
void print_all(std::string const &title, DatasetVector const &datasets) {
  std::cout << title << ":" << std::endl;
  for (auto const &d : datasets)
    std::cout << "  " << d->name << ", " << d->values.size() << " values, mean " << mean(*d) << std::endl;
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

  // 1. Read: every dataset is owned by the vector
  DatasetVector all;
  std::string line;
  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#')
      continue;
    std::istringstream iss{line};
    if (auto d = read_dataset(iss))
      all.push_back(std::move(d));
    else
      std::cerr << "Skipping invalid line: " << line << std::endl;
  }
  print_all("all", all);

  // 2. Transfer the datasets with mean > 10 to another vector:
  //    ownership moves, the moved-from pointers in "all" become empty
  DatasetVector large;
  for (auto &d : all)
    if (mean(*d) > 10.)
      large.push_back(std::move(d));
  std::erase_if(all, [](auto const &d) { return !d; });
  print_all("large (mean > 10)", large);
  print_all("remaining", all);

  // 3. A non-owning observer of the dataset with the largest mean
  auto it = std::max_element(large.begin(), large.end(),
                             [](auto const &a, auto const &b) { return mean(*a) < mean(*b); });
  if (it != large.end()) {
    Dataset const *largest = it->get();
    std::cout << "Largest mean: " << largest->name << std::endl;
  }
  return 0;
} // both vectors are destroyed here, and with them all the datasets
