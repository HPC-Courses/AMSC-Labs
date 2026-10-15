#include "dataset.hpp"

std::unique_ptr<Dataset> read_dataset(std::istream &in) {
  auto d = std::make_unique<Dataset>();
  if (!(in >> d->name))
    return nullptr;
  double v;
  while (in >> v)
    d->values.push_back(v);
  // stopped before the end of the line: something that is not a number
  if (!in.eof() || d->values.empty())
    return nullptr;
  return d;
}

double mean(const Dataset &d) {
  double sum = 0.;
  for (double v : d.values)
    sum += v;
  return sum / d.values.size();
}
