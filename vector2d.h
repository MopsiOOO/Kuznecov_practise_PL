#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>
#include <string>

class Vector2D {
public:
    Vector2D(double x = 0.0, double y = 0.0);
    double x() const;
    double y() const;
    virtual double length() const;

    Vector2D scale(double factor) const;
    Vector2D scale(double fx, double fy) const;
    void scale(double factor);

    bool operator==(const Vector2D& other) const;
    bool operator!=(const Vector2D& other) const;
    bool operator<(const Vector2D& other) const;
    bool operator>(const Vector2D& other) const;
    bool operator<=(const Vector2D& other) const;
    bool operator>=(const Vector2D& other) const;

    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator-(const Vector2D& other) const;
    Vector2D operator*(double factor) const;
    Vector2D operator/(double factor) const;
    Vector2D& operator+=(const Vector2D& other);
    Vector2D& operator-=(const Vector2D& other);
    Vector2D operator-() const;

    virtual std::string describe() const;

    friend Vector2D operator*(double factor, const Vector2D& v);

private:
    double x_;
    double y_;
};

std::string to_string(const Vector2D& v);
std::ostream& operator<<(std::ostream& os, const Vector2D& v);

#endif // VECTOR2D_H
