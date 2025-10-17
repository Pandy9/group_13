/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#include "ppm.hpp"
#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>

namespace PPM {

void Reader::fill(std::string filename)
{
    std::ifstream f {};

    f.open(filename);

    if (!f) {
        stream.setstate(std::ios::failbit);
        return;
    }

    std::copy(
        std::istreambuf_iterator<char> { f.rdbuf() },
        std::istreambuf_iterator<char> {},
        std::ostreambuf_iterator<char> { stream });

    f.close();
}

std::string Reader::get_magic_number()
{
    std::string magic {};

    std::getline(stream, magic);

    return magic;
}

std::pair<unsigned, unsigned> Reader::get_dimensions()
{
    std::string line {};

    while (std::getline(stream, line) && line[0] == '#')
        ;

    std::regex regex { "^(\\d+) (\\d+)$" };
    std::smatch matches {};

    std::regex_match(line, matches, regex);

    if (matches.ready()) {
        return { std::stoul(matches[1]), std::stoul(matches[2]) };
    } else {
        return { 0, 0 };
    }
}

unsigned Reader::get_color_max()
{
    std::string line {};

    std::getline(stream, line);

    std::regex regex { "^(\\d+)$" };
    std::smatch matches {};

    std::regex_match(line, matches, regex);

    if (matches.ready()) {
        return std::stoul(matches[1]);
    } else {
        return 0;
    }
}

std::tuple<unsigned char*, unsigned char*, unsigned char*> Reader::get_data(unsigned x_size, unsigned y_size)
{
    auto size { x_size * y_size };
    auto R { new char[size] }, G { new char[size] }, B { new char[size] };

    for (auto i { 0 }, read { 0 }; i < size; i++, read = 0) {
        stream.read(R + i, 1);
        read += stream.gcount();
        stream.read(G + i, 1);
        read += stream.gcount();
        stream.read(B + i, 1);
        read += stream.gcount();

        if (read != 3) {
            delete[] R;
            delete[] G;
            delete[] B;
            return { nullptr, nullptr, nullptr };
        }
    }

    return { reinterpret_cast<unsigned char*>(R), reinterpret_cast<unsigned char*>(G), reinterpret_cast<unsigned char*>(B) };
}

Matrix Reader::operator()(std::string filename)
{
    try {
        fill(filename);

        if (stream.fail()) {
            throw std::runtime_error { "couldn't open file " + filename };
        }

        auto magic { get_magic_number() };

        if (magic != magic_number) {
            throw std::runtime_error { "incorrect magic number: " + magic };
        }

        auto [x_size, y_size] { get_dimensions() };

        if (x_size == 0 || y_size == 0) {
            throw std::runtime_error { "couldn't read dimensions" };
        }

        auto total_size { x_size * y_size };

        if (total_size > max_pixels) {
            throw std::runtime_error { "image size is too big: " + std::to_string(total_size) };
        }

        auto color_max { get_color_max() };

        if (color_max == 0) {
            throw std::runtime_error { "couldn't read color max" };
        }

        auto [R, G, B] { get_data(x_size, y_size) };

        if (!R || !G || !B) {
            throw std::runtime_error { "couldn't read image data" };
        }

        stream.clear();
        return Matrix { R, G, B, x_size, y_size, color_max };
    } catch (std::runtime_error e) {
        error("reading", e.what());
        stream.clear();
        return Matrix {};
    }
}

void error(std::string op, std::string what)
{
    std::cerr << "Encountered PPM error during " << op << ": " << what << std::endl;
}

void Writer::operator()(Matrix m, std::string filename)
{
    try {
        std::ofstream f {};

        f.open(filename);

        if (!f) {
            throw std::runtime_error { "failed to open " + filename };
        }

        f << magic_number << std::endl;

        f << m.get_x_size() << " " << m.get_y_size() << std::endl;
        f << m.get_color_max() << std::endl;

        auto size { m.get_x_size() * m.get_y_size() };
        auto R { m.get_R() }, G { m.get_G() }, B { m.get_B() };
        auto it_R { R }, it_G { G }, it_B { B };

        while (it_R < R + size && it_G < G + size && it_B < B + size) {
            f << *it_R++
              << *it_G++
              << *it_B++;
        }

        f.close();
    } catch (std::runtime_error e) {
        error("writing", e.what());
    }
}

}
