/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#include "analysis_par.hpp"
#include <pthread.h>
#include <vector>
#include <algorithm>
#include <cmath>

namespace AnalysisPar
{

struct PearsonTask
{
    std::vector<Vector>* datasets;
    std::size_t start_i;
    std::size_t end_i;
    std::vector<double> results;
};

void* pearson_worker(void* arg)
{
    auto* task = static_cast<PearsonTask*>(arg);
    auto& data = *task->datasets;
    const std::size_t size = data.size();

    for (std::size_t i = task->start_i; i < task->end_i; ++i)
    {
        for (std::size_t j = i + 1; j < size; ++j)
        {
            double corr = pearson(data[i], data[j]);
            task->results.push_back(corr);
        }
    }
    return nullptr;
}

std::vector<double>
correlation_coefficients(std::vector<Vector>& datasets, int n_threads)
{
    const std::size_t size = datasets.size();
    if (size < 2)
        return {};

    if (n_threads < 1)
        n_threads = 1;
    if (static_cast<std::size_t>(n_threads) > size - 1)
        n_threads = size - 1;

    std::vector<pthread_t> threads(n_threads);
    std::vector<PearsonTask> tasks(n_threads);

    std::size_t work_per_thread = (size - 1) / n_threads;

    for (int i = 0; i < n_threads; ++i)
    {
        std::size_t start = i * work_per_thread;
        std::size_t end = (i == n_threads - 1)
            ? (size - 1)
            : (i + 1) * work_per_thread;

        tasks[i] = { &datasets, start, end, {} };
        pthread_create(&threads[i], nullptr,
                       pearson_worker, &tasks[i]);
    }

    std::vector<double> result;

    for (int i = 0; i < n_threads; ++i)
    {
        pthread_join(threads[i], nullptr);
        result.insert(result.end(),
                      tasks[i].results.begin(),
                      tasks[i].results.end());
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

    if (x_mag == 0.0 || y_mag == 0.0 ||
        !std::isfinite(x_mag) || !std::isfinite(y_mag)) {
        return 0.0;
    }

    double r = dot_sum / (x_mag * y_mag);

    if (!std::isfinite(r)) return 0.0;
    if (r > 1.0) r = 1.0;
    else if (r < -1.0) r = -1.0;

    return r;
}

}
