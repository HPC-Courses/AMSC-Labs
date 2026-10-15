#ifndef DATASET_HPP
#define DATASET_HPP

#include <cstddef>
#include <istream>
#include <string>
#include <vector>

// A named, non-empty series of measurements.
// Invariant: values_ is never empty, so mean() is always well defined.
// The data are private: the only way to change them is through the public methods,
// and none of them can break the invariant.
class Dataset {
public:
  // Establishes the invariant: throws std::invalid_argument if values is empty
  Dataset(const std::string &name, const std::vector<double> &values);

  // const methods: they do not change the object, so they can be called on a const Dataset
  const std::string &name() const { return name_; }
  std::size_t size() const { return values_.size(); }
  double mean() const;

  // non-const method: it changes the values, but not their number
  void scale(double factor);

private:
  std::string name_;
  std::vector<double> values_;
};

// Reads "<name> <v1> <v2> ..." from the stream and returns the Dataset BY VALUE:
// no pointer is needed, an invalid line throws instead of returning nullptr.
// Throws std::invalid_argument if a value is not a number or there are no values.
Dataset read_dataset(std::istream &in);

#endif /* DATASET_HPP */
