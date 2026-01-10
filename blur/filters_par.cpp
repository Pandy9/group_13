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
        Matrix *src;
        Matrix *dst;
        int radius;
        int start_y;
        int end_y;
        bool horizontal;
    };

    void *blur_section(void *arg)
    {
        auto *t = static_cast<BlurTask *>(arg);
        Matrix &src = *t->src;
        Matrix &dst = *t->dst;

        for (int y = t->start_y; y < t->end_y; ++y)
        {
            for (unsigned x = 0; x < dst.get_x_size(); ++x)
            {
                double w[Gauss::max_radius]{};
                Gauss::get_weights(t->radius, w);

                double r = w[0] * src.r(x, y);
                double g = w[0] * src.g(x, y);
                double b = w[0] * src.b(x, y);
                double n = w[0];

                for (int wi = 1; wi <= t->radius; ++wi)
                {
                    double wc = w[wi];

                    if (t->horizontal)
                    {
                        int x2 = static_cast<int>(x) - wi;
                        if (x2 >= 0)
                        {
                            r += wc * src.r(x2, y);
                            g += wc * src.g(x2, y);
                            b += wc * src.b(x2, y);
                            n += wc;
                        }
                        x2 = static_cast<int>(x) + wi;
                        if (x2 < static_cast<int>(src.get_x_size()))
                        {
                            r += wc * src.r(x2, y);
                            g += wc * src.g(x2, y);
                            b += wc * src.b(x2, y);
                            n += wc;
                        }
                    }
                    else
                    {
                        int y2 = y - wi;
                        if (y2 >= 0)
                        {
                            r += wc * src.r(x, y2);
                            g += wc * src.g(x, y2);
                            b += wc * src.b(x, y2);
                            n += wc;
                        }
                        y2 = y + wi;
                        if (y2 < static_cast<int>(src.get_y_size()))
                        {
                            r += wc * src.r(x, y2);
                            g += wc * src.g(x, y2);
                            b += wc * src.b(x, y2);
                            n += wc;
                        }
                    }
                }

                dst.r(x, y) = r / n;
                dst.g(x, y) = g / n;
                dst.b(x, y) = b / n;
            }
        }
        return nullptr;
    }

    Matrix blur_parallel(Matrix m, const int radius, int n_threads)
    {
        Matrix scratch{m};
        Matrix dst{m};

        int rows = dst.get_y_size();
        int rows_per_thread = rows / n_threads;

        std::vector<pthread_t> threads(n_threads);
        std::vector<BlurTask> tasks(n_threads);

        for (int i = 0; i < n_threads; ++i)
        {
            int start_y = i * rows_per_thread;
            int end_y = (i == n_threads - 1) ? rows : (i + 1) * rows_per_thread;

            tasks[i] = {&dst, &scratch, radius, start_y, end_y, true};
            pthread_create(&threads[i], nullptr, blur_section, &tasks[i]);
        }
        for (auto &t : threads)
            pthread_join(t, nullptr);

        for (int i = 0; i < n_threads; ++i)
        {
            tasks[i].src = &scratch;
            tasks[i].dst = &dst;
            tasks[i].horizontal = false;
            pthread_create(&threads[i], nullptr, blur_section, &tasks[i]);
        }
        for (auto &t : threads)
            pthread_join(t, nullptr);

        return dst;
    }
}
