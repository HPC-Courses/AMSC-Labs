#ifndef DATASETS_HPP
#define DATASETS_HPP

#include <istream>
#include <string>
#include <vector>

// A named series of measurements (an aggregate: public data, no constructors)
struct Dataset {
  std::string name;
  std::vector<double> values;
};

// Reads "<name> <v1> <v2> ..." from the stream.
// Returns a pointer to a new Dataset, or nullptr if the line has no values or a
// value is not a number.
// TODO: who owns the returned Dataset? Make it explicit in the return type
Dataset *read_dataset(std::istream &in);

double mean(Dataset const &d);

#endif /* DATASETS_HPP */
