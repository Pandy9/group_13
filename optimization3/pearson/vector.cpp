/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#include "vector.hpp"
#include <iostream>
#include <cmath>
#include <vector>

Vector::Vector()
    : size{0}, data{nullptr}
{
}

Vector::~Vector()
{
    if (data)
    {
        delete[] data;
    }

    size = 0;
}

Vector::Vector(unsigned size)
    : size{size}, data{new double[size]}
{
}

Vector::Vector(unsigned size, double *data)
    : size{size}, data{data}
{
}

Vector::Vector(const Vector &other)
    : Vector{other.size}
{
    for (unsigned int i{0}; i < size; i++)
    {
        data[i] = other.data[i];
    }
}

unsigned Vector::get_size() const
{
    return size;
}

double *Vector::get_data()
{
    return data;
}

double Vector::operator[](unsigned i) const
{
    return data[i];
}

double &Vector::operator[](unsigned i)
{
    return data[i];
}

double Vector::mean() const
{
    double sum{0};

    for (unsigned int i{0}; i < size; i++)
    {
        sum += data[i];
    }

    return sum / static_cast<double>(size);
}

double Vector::magnitude() const
{
    double dot_prod{dot(*this)};
    return std::sqrt(dot_prod);
}

Vector Vector::operator/(const double& div)
{
    auto result{*this};

    for (unsigned int i{0}; i < size; i++)
    {
        result[i] /= div;
    }

    return result;
}

Vector Vector::operator-(const double& sub)
{
    Vector result{*this};

    for (unsigned int i{0}; i < size; i++)
    {
        result[i] -= sub;
    }

    return result;
}

double Vector::dot(const Vector& rhs) const
{
    double result{0};

    for (unsigned int i{0}; i < size; i++)
    {
        result += data[i] * rhs[i];
    }

    return result;
}