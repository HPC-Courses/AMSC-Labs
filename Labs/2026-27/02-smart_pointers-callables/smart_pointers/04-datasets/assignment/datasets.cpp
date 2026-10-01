#include "datasets.hpp"

#include <numeric>

Dataset *read_dataset(std::istream &in) {
  Dataset *d = new Dataset; // TODO: no naked new
  if (!(in >> d->name))
    return nullptr;
  double v;
  while (in >> v)
    d->values.push_back(v);
  // stopped before the end of the line: something that is not a number
  if (!in.eof() || d->values.empty())
    return nullptr; // BUG: what happens to d?
  return d;
}

double mean(Dataset const &d) {
  return std::accumulate(d.values.begin(), d.values.end(), 0.) / static_cast<double>(d.values.size());
}
