/*
Author: David Holmqvist <daae19@student.bth.se>
Parallel main program for Gaussian blur
*/

#include "matrix.hpp"
#include "ppm.hpp"
#include "filters.hpp"
#include <cstdlib>
#include <iostream>

int main(int argc, char const* argv[])
{
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " [radius] [infile] [outfile] [threads]" << std::endl;
        std::exit(1);
    }

    PPM::Reader reader {};
    PPM::Writer writer {};

    auto m { reader(argv[2]) };
    auto radius { static_cast<unsigned>(std::stoul(argv[1])) };
    int n_threads = std::stoi(argv[4]);

    std::cout << "Running parallel blur with " << n_threads << " threads..." << std::endl;

    auto blurred = Filter::blur_parallel(m, radius, n_threads);

    writer(blurred, argv[3]);
    std::cout << "Output written to " << argv[3] << std::endl;

    return 0;
}
