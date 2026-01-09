#pragma once

#include "vector.hpp"
#include <vector>

namespace AnalysisPar
{
    std::vector<double>
    correlation_coefficients(std::vector<Vector> datasets, int n_threads);

    double pearson(Vector vec1, Vector vec2);
}
