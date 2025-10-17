/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#include "matrix.hpp"

#if !defined(FILTERS_HPP)
#define FILTERS_HPP

namespace Filter
{

    namespace Gauss
    {
        constexpr unsigned max_radius{1000};
        constexpr float max_x{1.33};
        constexpr float pi{3.14159};

        void get_weights(int n, double *weights_out);
    }

    Matrix blur(Matrix m, const int radius);

}

#endif