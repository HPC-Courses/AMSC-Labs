#include "dataset.hpp"

#include <stdexcept>

// The members are initialised in the initialisation list; the body checks the invariant
Dataset::Dataset(const std::string &name, const std::vector<double> &values)
    : name_{name}, values_{values} {
  if (values_.empty())
    throw std::invalid_argument("dataset '" + name_ + "' has no values");
}

double Dataset::mean() const {
  double sum = 0.;
  for (double v : values_)
    sum += v;
  return sum / values_.size();
}

void Dataset::scale(double factor) {
  for (double &v : values_)
    v *= factor;
}

Dataset read_dataset(std::istream &in) {
  std::string name;
  in >> name;
  std::vector<double> values;
  double v;
  while (in >> v)
    values.push_back(v);
  // stopped before the end of the line: something that is not a number
  if (!in.eof())
    throw std::invalid_argument("dataset '" + name + "' contains a value that is not a number");
  // the constructor checks that values is not empty
  return Dataset{name, values};
}
