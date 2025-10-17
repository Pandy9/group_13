/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#include "matrix.hpp"
#include "ppm.hpp"
#include <fstream>
#include <stdexcept>

Matrix::Matrix(unsigned char* R, unsigned char* G, unsigned char* B, unsigned x_size, unsigned y_size, unsigned color_max)
    : R { R }
    , G { G }
    , B { B }
    , x_size { x_size }
    , y_size { y_size }
    , color_max { color_max }
{
}

Matrix::Matrix()
    : Matrix {
        nullptr,
        nullptr,
        nullptr,
        0,
        0,
        0,
    }
{
}

Matrix::Matrix(unsigned dimension)
    : R { new unsigned char[dimension * dimension] }
    , G { new unsigned char[dimension * dimension] }
    , B { new unsigned char[dimension * dimension] }
    , x_size { dimension }
    , y_size { dimension }
    , color_max { 0 }
{
}

Matrix::Matrix(const Matrix& other)
    : R { new unsigned char[other.x_size * other.y_size] }
    , G { new unsigned char[other.x_size * other.y_size] }
    , B { new unsigned char[other.x_size * other.y_size] }
    , x_size { other.x_size }
    , y_size { other.y_size }
    , color_max { other.color_max }
{
    for (auto x { 0 }; x < x_size; x++) {
        for (auto y { 0 }; y < y_size; y++) {
            auto &r_val { r(x, y) }, &g_val { g(x, y) }, &b_val { b(x, y) };
            auto other_r_val { other.r(x, y) }, other_g_val { other.g(x, y) }, other_b_val { other.b(x, y) };

            r_val = other_r_val;
            g_val = other_g_val;
            b_val = other_b_val;
        }
    }
}

Matrix& Matrix::operator=(const Matrix other)
{
    if (this == &other) {
        return *this;
    }

    this->~Matrix();

    R = new unsigned char[other.x_size * other.y_size];
    G = new unsigned char[other.x_size * other.y_size];
    B = new unsigned char[other.x_size * other.y_size];

    x_size = other.x_size;
    y_size = other.y_size;
    color_max = other.color_max;

    for (auto x { 0 }; x < x_size; x++) {
        for (auto y { 0 }; y < y_size; y++) {
            auto &r_val { r(x, y) }, &g_val { g(x, y) }, &b_val { b(x, y) };
            auto other_r_val { other.r(x, y) }, other_g_val { other.g(x, y) }, other_b_val { other.b(x, y) };

            r_val = other_r_val;
            g_val = other_g_val;
            b_val = other_b_val;
        }
    }

    return *this;
}

Matrix::~Matrix()
{
    if (R) {
        delete[] R;
        R = nullptr;
    }
    if (G) {
        delete[] G;
        G = nullptr;
    }
    if (B) {
        delete[] B;
        B = nullptr;
    }

    x_size = y_size = color_max = 0;
}

unsigned Matrix::get_x_size() const
{
    return x_size;
}

unsigned Matrix::get_y_size() const
{
    return y_size;
}

unsigned Matrix::get_color_max() const
{
    return color_max;
}

unsigned char const* Matrix::get_R() const
{
    return R;
}

unsigned char const* Matrix::get_G() const
{
    return G;
}

unsigned char const* Matrix::get_B() const
{
    return B;
}

unsigned char Matrix::r(unsigned x, unsigned y) const
{
    return R[y * x_size + x];
}

unsigned char Matrix::g(unsigned x, unsigned y) const
{
    return G[y * x_size + x];
}

unsigned char Matrix::b(unsigned x, unsigned y) const
{
    return B[y * x_size + x];
}

unsigned char& Matrix::r(unsigned x, unsigned y)
{
    return R[y * x_size + x];
}

unsigned char& Matrix::g(unsigned x, unsigned y)
{
    return G[y * x_size + x];
}

unsigned char& Matrix::b(unsigned x, unsigned y)
{
    return B[y * x_size + x];
}
