#include <iostream>
#include <string>

#include "vector2d.h"
#include "vector3d.h"
#include "shape.h"
#include "circle.h"
#include "rectangle.h"
#include "triangle.h"

int main() {
    Vector2D v1(3.0, 4.0);
    Vector2D v2(1.0, 2.0);
    std::cout << v1 << std::endl;
    std::cout << v2 << std::endl;
    std::cout << "v1 = " << v1 << ", v2 = " << v2 << std::endl;
    std::cout << "via to_string: " << to_string(v1) << std::endl;
    std::cout << "via operator<<: " << v1 << std::endl;

    Vector2D v2d(3.0, 4.0);
    Vector3D v3(1.0, 2.0, 2.0);
    Vector2D* ptr1 = &v2d;
    Vector2D* ptr2 = &v3;
    std::cout << "ptr1->length() = " << ptr1->length() << std::endl;
    std::cout << "ptr2->length() = " << ptr2->length() << std::endl;

    Vector2D* vectors[] = { &v2d, &v3 };
    for (Vector2D* p : vectors) {
        std::cout << p->describe() << std::endl;
    }

    Circle c(5.0);
    Rectangle r(4.0, 6.0);
    Triangle t(3.0, 4.0, 5.0);
    Shape* shapes[] = { &c, &r, &t };
    for (Shape* s : shapes) {
        std::cout << *s << std::endl;
    }

    Shape* s = new Circle(5.0);
    delete s;

    return 0;
}
