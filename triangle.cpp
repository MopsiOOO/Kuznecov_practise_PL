#include "triangle.h"

#include <cmath>

Triangle::Triangle(double a, double b, double c)
    : a_(a)
    , b_(b)
    , c_(c)
{
}
double Triangle::area() const {
    const double p = perimeter() / 2.0;
    return std::sqrt(p * (p - a_) * (p - b_) * (p - c_));
}
double Triangle::perimeter() const {
    return a_ + b_ + c_;
}
std::string Triangle::name() const {
    return "Triangle";
}
