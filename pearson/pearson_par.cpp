/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#include "analysis_par.hpp"
#include "dataset.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char const* argv[])
{
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0]
                  << " [dataset] [outfile] [threads]" << std::endl;
        std::exit(1);
    }

    auto datasets { Dataset::read(argv[1]) };
    int n_threads = std::stoi(argv[3]);

    std::cout << "Running parallel Pearson with "
              << n_threads << " threads..." << std::endl;

    auto corrs {
        AnalysisPar::correlation_coefficients(datasets, n_threads)
    };

    Dataset::write(corrs, argv[2]);
    return 0;
}
