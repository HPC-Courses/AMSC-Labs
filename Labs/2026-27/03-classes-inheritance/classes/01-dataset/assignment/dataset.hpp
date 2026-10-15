#ifndef DATASET_HPP
#define DATASET_HPP

#include <istream>
#include <memory>
#include <string>
#include <vector>

// The Dataset of Lab 2: an aggregate, all data are public.
// TODO: turn it into a class with the invariant "values is never empty":
//       - private data members name_ and values_;
//       - a constructor Dataset(name, values) that initialises them in the
//         initialisation list, and throws std::invalid_argument if values is empty;
//       - const methods name(), size() and mean();
//       - a non-const method scale(factor), which multiplies all the values by factor.
struct Dataset {
  std::string name;
  std::vector<double> values;
};

// TODO: a Dataset is a small object and an invalid line can throw, so there is
//       no need for a pointer: return it BY VALUE, and throw std::invalid_argument
//       (instead of returning nullptr) if the line is not valid
std::unique_ptr<Dataset> read_dataset(std::istream &in);

// TODO: make it a const member function
double mean(const Dataset &d);

#endif /* DATASET_HPP */
