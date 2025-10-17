/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#if !defined(VECTOR_HPP)
#define VECTOR_HPP

class Vector {
private:
    unsigned size;
    double* data;

public:
    Vector();
    Vector(unsigned size);
    Vector(unsigned size, double* data);
    Vector(const Vector& other);
    ~Vector();

    double magnitude() const;
    double mean() const;
    double normalize() const;
    double dot(Vector rhs) const;

    unsigned get_size() const;
    double* get_data();

    Vector operator/(double div);
    Vector operator-(double sub);
    double operator[](unsigned i) const;
    double& operator[](unsigned i);
};

#endif