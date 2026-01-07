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
        double *weights;
        int radius;
        int start_y;
        int end_y;
        bool horizontal; 
    };

    
    void *blur_section(void *arg)
    {
        BlurTask *t = static_cast<BlurTask *>(arg);
        Matrix &src = *t->src;
        Matrix &dst = *t->dst;
        double *w = t->weights;
        int r = t->radius;

        if (t->horizontal)
        {
            for (int y = t->start_y; y < t->end_y; ++y)
            {
                for (unsigned x = 0; x < dst.get_x_size(); ++x)
                {
                    double rr = w[0] * src.r(x, y);
                    double gg = w[0] * src.g(x, y);
                    double bb = w[0] * src.b(x, y);
                    double n = w[0];

                    for (int wi = 1; wi <= r; ++wi)
                    {
                        double wc = w[wi];
                        int x2 = static_cast<int>(x) - wi;
                        if (x2 >= 0)
                        {
                            rr += wc * src.r(x2, y);
                            gg += wc * src.g(x2, y);
                            bb += wc * src.b(x2, y);
                            n += wc;
                        }
                        x2 = static_cast<int>(x) + wi;
                        if (x2 < static_cast<int>(src.get_x_size()))
                        {
                            rr += wc * src.r(x2, y);
                            gg += wc * src.g(x2, y);
                            bb += wc * src.b(x2, y);
                            n += wc;
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
            for (int y = t->start_y; y < t->end_y; ++y)
            {
                for (unsigned x = 0; x < dst.get_x_size(); ++x)
                {
                    double rr = w[0] * src.r(x, y);
                    double gg = w[0] * src.g(x, y);
                    double bb = w[0] * src.b(x, y);
                    double n = w[0];

                    for (int wi = 1; wi <= r; ++wi)
                    {
                        double wc = w[wi];
                        int y2 = y - wi;
                        if (y2 >= 0)
                        {
                            rr += wc * src.r(x, y2);
                            gg += wc * src.g(x, y2);
                            bb += wc * src.b(x, y2);
                            n += wc;
                        }
                        y2 = y + wi;
                        if (y2 < static_cast<int>(src.get_y_size()))
                        {
                            rr += wc * src.r(x, y2);
                            gg += wc * src.g(x, y2);
                            bb += wc * src.b(x, y2);
                            n += wc;
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

    Matrix blur_parallel_internal(Matrix &m, const int radius, int n_threads)
    {
        Matrix scratch{m};
        Matrix dst{m};

        double w[Gauss::max_radius]{};
        Gauss::get_weights(radius, w);

        std::vector<pthread_t> threads(n_threads);
        std::vector<BlurTask> tasks(n_threads);

        int rows_per_thread = dst.get_y_size() / n_threads;

    for (int i = 0; i < n_threads; ++i)
    {
        int start_y = i * rows_per_thread;
        int end_y = (i == n_threads - 1)
              ? dst.get_y_size()
              : (i + 1) * rows_per_thread;

        tasks[i] = {
            &dst,
            &scratch,
            w,
            radius,
            start_y,
            end_y,
            true
        };
        pthread_create(&threads[i], nullptr, blur_section, &tasks[i]);
    }
    for (int i = 0; i < n_threads; ++i)
        pthread_join(threads[i], nullptr);

    for (int i = 0; i < n_threads; ++i)
    {
        int start_y = i * rows_per_thread;
        int end_y = (i == n_threads - 1)
              ? dst.get_y_size()
              : (i + 1) * rows_per_thread;

        tasks[i] = {
            &scratch,
            &dst,
            w,
            radius,
            start_y,
            end_y,
            false
        };
        pthread_create(&threads[i], nullptr, blur_section, &tasks[i]);
    }
    for (int i = 0; i < n_threads; ++i)
        pthread_join(threads[i], nullptr);

        return dst;
    }

    Matrix blur_parallel(Matrix m, const int radius, int n_threads)
    {
        return blur_parallel_internal(m, radius, n_threads);
    }

} 
