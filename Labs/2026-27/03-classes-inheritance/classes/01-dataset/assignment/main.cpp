#include "dataset.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// TODO: write print(const Dataset &), which prints name, size and mean

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

  // TODO: a std::vector<Dataset> is enough. Skip the invalid lines with try/catch
  std::vector<std::unique_ptr<Dataset>> all;
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

  std::cout << "datasets:" << std::endl;
  for (const auto &d : all)
    std::cout << "  " << d->name << ", " << d->values.size() << " values, mean " << mean(*d) << std::endl;

  // With public data nothing prevents this. What does mean() return now?
  all[0]->values.clear(); // equivalent to all.front()->values.clear();
  std::cout << "  " << all[0]->name << " after clear(): mean " << mean(*all[0]) << std::endl;

  // TODO: strain is in mm/m: convert it to m/m with scale(1e-3), and print it again
  return 0;
}
