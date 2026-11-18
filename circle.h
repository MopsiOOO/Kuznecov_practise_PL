#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"

class Circle : public Shape {
public:
    Circle(double radius);
    ~Circle();
    double area() const override;
    double perimeter() const override;
    std::string name() const override;
private:
    double radius_;
};

#endif // CIRCLE_H
