/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#include "matrix.hpp"
#include <exception>
#include <iostream>
#include <sstream>

#if !defined(PPM_READER_HPP)
#define PPM_READER_HPP

namespace PPM {

void error(std::string op, std::string what);

constexpr unsigned max_dimension { 3000 };
constexpr unsigned max_pixels { max_dimension * max_dimension };
constexpr char const* magic_number { "P6" };

class Reader {
private:
    std::stringstream stream;

    std::string get_magic_number();
    std::pair<unsigned, unsigned> get_dimensions();
    std::tuple<unsigned char*, unsigned char*, unsigned char*> get_data(unsigned x_size, unsigned y_size);
    unsigned get_color_max();
    void fill(std::string filename);

public:
    Matrix operator()(std::string filename);
};

class Writer {
public:
    void operator()(Matrix m, std::string filename);
};

}

#endif
