/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#include "filters.hpp"
#include "matrix.hpp"
#include "ppm.hpp"
#include <pthread.h>
#include <vector>
#include <cmath>

namespace Filter
{
    namespace Gauss
    {
        extern void get_weights(int n, double *weights_out);
    }

    struct BlurTask
    {
        Matrix* src;
        Matrix* dst;
        const double* w;
        int radius;
        int start_x;
        int end_x;
        bool horizontal;
    };

    void* blur_worker(void* arg)
    {
        auto* t = static_cast<BlurTask*>(arg);
        auto& src = *t->src;
        auto& dst = *t->dst;
        const double* w = t->w;
        int r = t->radius;

        if (t->horizontal)
        {
            for (int x = t->start_x; x < t->end_x; ++x)
            {
                for (unsigned y = 0; y < dst.get_y_size(); ++y)
                {
                    double rr = w[0] * src.r(x, y);
                    double gg = w[0] * src.g(x, y);
                    double bb = w[0] * src.b(x, y);
                    double n  = w[0];

                    for (int wi = 1; wi <= r; ++wi)
                    {
                        int x2 = x - wi;
                        if (x2 >= 0)
                        {
                            double wc = w[wi];
                            rr += wc * src.r(x2, y);
                            gg += wc * src.g(x2, y);
                            bb += wc * src.b(x2, y);
                            n  += wc;
                        }
                        x2 = x + wi;
                        if (x2 < int(src.get_x_size()))
                        {
                            double wc = w[wi];
                            rr += wc * src.r(x2, y);
                            gg += wc * src.g(x2, y);
                            bb += wc * src.b(x2, y);
                            n  += wc;
                        }
                    }
                    dst.r(x, y) = rr / n;
                    dst.g(x, y) = gg / n;
                    dst.b(x, y) = bb / n;
                }
            }
        }
        else
        {
            for (int x = t->start_x; x < t->end_x; ++x)
            {
                for (unsigned y = 0; y < dst.get_y_size(); ++y)
                {
                    double rr = w[0] * src.r(x, y);
                    double gg = w[0] * src.g(x, y);
                    double bb = w[0] * src.b(x, y);
                    double n  = w[0];

                    for (int wi = 1; wi <= r; ++wi)
                    {
                        int y2 = int(y) - wi;
                        if (y2 >= 0)
                        {
                            double wc = w[wi];
                            rr += wc * src.r(x, y2);
                            gg += wc * src.g(x, y2);
                            bb += wc * src.b(x, y2);
                            n  += wc;
                        }
                        y2 = int(y) + wi;
                        if (y2 < int(src.get_y_size()))
                        {
                            double wc = w[wi];
                            rr += wc * src.r(x, y2);
                            gg += wc * src.g(x, y2);
                            bb += wc * src.b(x, y2);
                            n  += wc;
                        }
                    }
                    dst.r(x, y) = rr / n;
                    dst.g(x, y) = gg / n;
                    dst.b(x, y) = bb / n;
                }
            }
        }
        return nullptr;
    }

    Matrix blur_parallel(Matrix m, const int radius, int n_threads)
    {
        Matrix scratch{m};
        Matrix dst{m};

        double w[Gauss::max_radius]{};
        Gauss::get_weights(radius, w);

        int width = dst.get_x_size();
        if (n_threads < 1) n_threads = 1;
        if (n_threads > width) n_threads = width;

        std::vector<pthread_t> threads(n_threads);
        std::vector<BlurTask> tasks(n_threads);

        int cols = width / n_threads;

        for (int i = 0; i < n_threads; ++i)
        {
            int start = i * cols;
            int end = (i == n_threads - 1) ? width : (i + 1) * cols;
            tasks[i] = { &dst, &scratch, w, radius, start, end, true };
            pthread_create(&threads[i], nullptr, blur_worker, &tasks[i]);
        }
        for (auto& t : threads) pthread_join(t, nullptr);

        for (int i = 0; i < n_threads; ++i)
        {
            int start = i * cols;
            int end = (i == n_threads - 1) ? width : (i + 1) * cols;
            tasks[i] = { &scratch, &dst, w, radius, start, end, false };
            pthread_create(&threads[i], nullptr, blur_worker, &tasks[i]);
        }
        for (auto& t : threads) pthread_join(t, nullptr);

        return dst;
    }
}
