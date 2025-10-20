/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#include "vector.hpp"
#include <string>
#include <vector>

#if !defined(DATASET_HPP)
#define DATASET_HPP

namespace Dataset
{
    std::vector<Vector> read(const std::string& filename);
    void write(const std::vector<double>& data, const std::string& filename);
};

#endif
