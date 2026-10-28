#include <iostream>
#include <stdexcept>

#include "vector2d.h"

void print(const Vector2D& v) {
    std::cout << "(" << v.x() << ", " << v.y() << ")" << std::endl;
}

int main() {
    Vector2D v(3.0, 4.0);
    std::cout << "v = (" << v.x() << ", " << v.y() << ")" << std::endl;
    std::cout << "length = " << v.length() << std::endl;

    Vector2D s(2.0, 3.0);
    const Vector2D& cs = s;
    std::cout << "scale(2.0) -> ";
    print(cs.scale(2.0));
    std::cout << "scale(2.0, 10.0) -> ";
    print(s.scale(2.0, 10.0));
    s.scale(5.0);
    std::cout << "scale(5.0) in-place -> ";
    print(s);

    Vector2D a(3.0, 4.0);
    Vector2D b(6.0, 8.0);
    Vector2D c(5.0, 0.0);
    std::cout << std::boolalpha;
    std::cout << "a == c: " << (a == c) << std::endl;
    std::cout << "a != b: " << (a != b) << std::endl;
    std::cout << "a < b: " << (a < b) << std::endl;
    std::cout << "b > a: " << (b > a) << std::endl;
    std::cout << "a <= c: " << (a <= c) << std::endl;
    std::cout << "b >= a: " << (b >= a) << std::endl;

    Vector2D p(1.0, 2.0);
    Vector2D q(3.0, 4.0);
    std::cout << "p + q = ";
    print(p + q);
    std::cout << "p - q = ";
    print(p - q);
    std::cout << "p * 3.0 = ";
    print(p * 3.0);
    std::cout << "p / 2.0 = ";
    print(p / 2.0);
    std::cout << "-q = ";
    print(-q);
    p += q;
    std::cout << "p += q -> ";
    print(p);
    p -= q;
    std::cout << "p -= q -> ";
    print(p);
    try {
        print(q / 0.0);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Ошибка деления: " << e.what() << std::endl;
    }

    Vector2D m(1.5, 2.5);
    std::cout << "m * 2.0 = ";
    print(m * 2.0);
    std::cout << "2.0 * m = ";
    print(2.0 * m);

    return 0;
}
