#ifndef DATASETS_HPP
#define DATASETS_HPP

#include <istream>
#include <memory>
#include <string>
#include <vector>

// A named series of measurements (an aggregate: public data, no constructors)
struct Dataset {
  std::string name;
  std::vector<double> values;
};

// Reads "<name> <v1> <v2> ..." from the stream.
// Ownership of the new Dataset is handed to the caller.
// Returns an empty pointer if the line has no values or a value is not a number.
std::unique_ptr<Dataset> read_dataset(std::istream &in);

// Only reads the dataset: no ownership involved, a const reference is enough
double mean(Dataset const &d);

#endif /* DATASETS_HPP */
