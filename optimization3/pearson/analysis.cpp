/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#include "analysis.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <list>
#include <vector>

namespace Analysis {

std::vector<double> correlation_coefficients(std::vector<Vector>& datasets)
{
    std::vector<double> result {};
    const std::size_t size = datasets.size();
    std::size_t sample1;
    std::size_t sample2;
    for (sample1 = 0; sample1 < size - 1; sample1++) {
        for (sample2 = sample1 + 1; sample2 < size; sample2++) {
            auto corr { pearson(datasets[sample1], datasets[sample2]) };
            result.push_back(corr);
        }
    }

    return result;
}

double pearson(Vector& vec1, Vector& vec2)
{
    const unsigned int n = vec1.get_size();
    if (n == 0 || vec2.get_size() != n) {
        return 0.0;
    }

    double sum_x = 0.0;
    double sum_y = 0.0;
    for (unsigned int i = 0; i < n; i++)
    {
        sum_x += vec1[i];
        sum_y += vec2[i];
    }

    double dn = static_cast<double>(n);
    double x_mean = sum_x / dn;
    double y_mean = sum_y / dn;

    double x_mag = 0.0;
    double y_mag = 0.0;
    double dot_sum = 0.0;

    for (unsigned int i = 0; i < n; i++)
    {
        const double xm = vec1[i] - x_mean;
        const double ym = vec2[i] - y_mean;

        x_mag += xm * xm;
        y_mag += ym * ym;

        dot_sum += xm * ym;
    }
    x_mag = std::sqrt(x_mag);
    y_mag = std::sqrt(y_mag);

    if (x_mag == 0.0 || y_mag == 0.0 || !std::isfinite(x_mag) || !std::isfinite(y_mag)) {
        return 0.0;
    }
    
    double r = dot_sum / (x_mag * y_mag);

    if (!std::isfinite(r)) 
        return 0.0;
    if (r > 1.0) 
        r = 1.0;
    else if (r < -1.0) 
        r = -1.0;

    return r;
}
};
