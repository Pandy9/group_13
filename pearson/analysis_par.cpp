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
    const std::vector<Vector>* datasets;
    int start_i;
    int end_i;
    std::vector<double> results;
};

void* pearson_worker(void* arg)
{
    auto* task = static_cast<PearsonTask*>(arg);
    const auto& data = *task->datasets;
    int n = data.size();

    for (int i = task->start_i; i < task->end_i; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            auto corr = pearson(data[i], data[j]);
            task->results.push_back(corr);
        }
    }
    return nullptr;
}

std::vector<double>
correlation_coefficients(std::vector<Vector> datasets, int n_threads)
{
    int n = datasets.size();
    if (n_threads > n - 1)
        n_threads = n - 1;
    if (n_threads < 1)
        n_threads = 1;

    std::vector<pthread_t> threads(n_threads);
    std::vector<PearsonTask> tasks(n_threads);

    int work_per_thread = (n - 1) / n_threads;

    for (int i = 0; i < n_threads; ++i)
    {
        int start = i * work_per_thread;
        int end = (i == n_threads - 1)
                    ? (n - 1)
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

double pearson(Vector vec1, Vector vec2)
{
    auto x_mean = vec1.mean();
    auto y_mean = vec2.mean();

    auto x_mm = vec1 - x_mean;
    auto y_mm = vec2 - y_mean;

    auto x_mag = x_mm.magnitude();
    auto y_mag = y_mm.magnitude();

    auto x_mm_over_x_mag = x_mm / x_mag;
    auto y_mm_over_y_mag = y_mm / y_mag;

    auto r = x_mm_over_x_mag.dot(y_mm_over_y_mag);

    return std::max(std::min(r, 1.0), -1.0);
}

}
