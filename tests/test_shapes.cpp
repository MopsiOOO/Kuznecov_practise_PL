// Unit tests for Shapes, Vector2D and Vector3D (practise-7): виртуальные функции и наследование.
// Self-contained: no external test framework required.
// Build: g++ -std=c++17 -Wall -Wextra -pedantic shape.cpp circle.cpp rectangle.cpp triangle.cpp \
//          vector2d.cpp vector3d.cpp tests/test_shapes.cpp -o run_tests

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

#include "../circle.h"
#include "../rectangle.h"
#include "../shape.h"
#include "../triangle.h"
#include "../vector2d.h"
#include "../vector3d.h"

namespace {

constexpr double kPi = 3.14159265358979323846;

int g_failures = 0;
int g_checks = 0;

void check(bool condition, const std::string& name) {
    ++g_checks;
    if (condition) {
        std::cout << "[PASS] " << name << '\n';
    } else {
        ++g_failures;
        std::cerr << "[FAIL] " << name << '\n';
    }
}

bool nearlyEqual(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) < eps;
}

void testCircle() {
    Circle c(1.0);
    check(nearlyEqual(c.area(), kPi), "Circle(1): area == pi");
    check(nearlyEqual(c.perimeter(), 2.0 * kPi), "Circle(1): perimeter == 2*pi");
    check(c.name() == "Circle", "Circle: name()");

    Circle c2(3.0);
    check(nearlyEqual(c2.area(), kPi * 9.0), "Circle(3): area == 9*pi");
}

void testRectangle() {
    Rectangle r(2.0, 3.0);
    check(nearlyEqual(r.area(), 6.0), "Rectangle(2, 3): area == 6");
    check(nearlyEqual(r.perimeter(), 10.0), "Rectangle(2, 3): perimeter == 10");
    check(r.name() == "Rectangle", "Rectangle: name()");
}

void testTriangle() {
    Triangle t(3.0, 4.0, 5.0);
    check(nearlyEqual(t.perimeter(), 12.0), "Triangle(3, 4, 5): perimeter == 12");
    check(nearlyEqual(t.area(), 6.0), "Triangle(3, 4, 5): area == 6 (формула Герона)");
    check(t.name() == "Triangle", "Triangle: name()");
}

void testShapePolymorphism() {
    Circle c(1.0);
    Rectangle r(2.0, 3.0);

    Shape* shapes[] = {&c, &r};
    check(nearlyEqual(shapes[0]->area(), kPi), "полиморфизм: Shape* -> Circle::area()");
    check(nearlyEqual(shapes[1]->area(), 6.0), "полиморфизм: Shape* -> Rectangle::area()");

    // describe() невиртуальный в Shape, но использует виртуальные name/area/perimeter
    check(shapes[0]->describe().find("Circle: area = ") == 0,
          "Shape::describe() использует виртуальные методы Circle");
    check(shapes[1]->describe().find("Rectangle: area = ") == 0,
          "Shape::describe() использует виртуальные методы Rectangle");

    std::ostringstream oss;
    oss << c;
    check(oss.str() == c.describe(), "operator<< выводит describe()");
}

void testVector2D() {
    Vector2D v(3.0, 4.0);
    check(nearlyEqual(v.length(), 5.0), "Vector2D(3, 4): length == 5");
    check(v.describe() == "2D vector (3.000000, 4.000000)", "Vector2D: describe()");
    check(to_string(v) == "(3.000000, 4.000000)", "Vector2D: to_string()");

    std::ostringstream oss;
    oss << v;
    check(oss.str() == "(3.000000, 4.000000)", "Vector2D: operator<<");

    Vector2D sum = v + Vector2D(1.0, 1.0);
    check(nearlyEqual(sum.x(), 4.0) && nearlyEqual(sum.y(), 5.0), "Vector2D: operator+");
}

void testVector3D() {
    Vector3D v(1.0, 2.0, 2.0);
    check(nearlyEqual(v.x(), 1.0) && nearlyEqual(v.y(), 2.0) && nearlyEqual(v.z(), 2.0),
          "Vector3D: геттеры x(), y(), z()");
    check(nearlyEqual(v.length(), 3.0), "Vector3D(1, 2, 2): length == 3 (учитывает z)");
    check(v.describe() == "3D vector (1.000000, 2.000000, 2.000000)", "Vector3D: describe()");

    Vector3D def;
    check(nearlyEqual(def.length(), 0.0), "Vector3D по умолчанию: length == 0");

    // length() виртуальный: через ссылку на Vector2D вызывается Vector3D::length()
    Vector2D& as2d = v;
    check(nearlyEqual(as2d.length(), 3.0), "полиморфизм: Vector2D& -> Vector3D::length()");

    const Vector2D plain(1.0, 2.0);
    check(nearlyEqual(plain.length(), std::sqrt(5.0)), "Vector2D::length() без z");
}

} // namespace

int main() {
    testCircle();
    testRectangle();
    testTriangle();
    testShapePolymorphism();
    testVector2D();
    testVector3D();

    std::cout << "\nИтого: " << g_checks << " проверок, "
              << g_failures << " провалено\n";

    return g_failures == 0 ? 0 : 1;
}
