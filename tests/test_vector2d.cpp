// Unit tests for the Vector2D class (practise-6): перегрузка операторов.
// Self-contained: no external test framework required.
// Build: g++ -std=c++17 -Wall -Wextra -pedantic vector2d.cpp tests/test_vector2d.cpp -o run_tests

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

#include "../vector2d.h"

namespace {

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

bool sameVector(const Vector2D& v, double x, double y) {
    return nearlyEqual(v.x(), x) && nearlyEqual(v.y(), y);
}

template <typename Fn>
bool throwsInvalidArgument(Fn&& fn) {
    try {
        fn();
    } catch (const std::invalid_argument&) {
        return true;
    } catch (...) {
        return false;
    }
    return false;
}

void testConstructorAndGetters() {
    Vector2D zero;
    check(sameVector(zero, 0.0, 0.0), "конструктор по умолчанию -> (0, 0)");

    Vector2D v(3.0, 4.0);
    check(sameVector(v, 3.0, 4.0), "геттеры x() и y()");
    check(nearlyEqual(v.length(), 5.0), "length() для (3, 4) == 5");
    check(nearlyEqual(Vector2D(1.0, 1.0).length(), std::sqrt(2.0)),
          "length() для (1, 1) == sqrt(2)");
}

void testScale() {
    const Vector2D v(1.0, 2.0);

    Vector2D scaled = v.scale(2.0);
    check(sameVector(scaled, 2.0, 4.0), "const scale(factor) возвращает масштабированную копию");
    check(sameVector(v, 1.0, 2.0), "const scale(factor) не меняет оригинал");

    Vector2D scaled2 = v.scale(2.0, 3.0);
    check(sameVector(scaled2, 2.0, 6.0), "scale(fx, fy) масштабирует по осям");

    Vector2D mutable_v(1.0, 2.0);
    mutable_v.scale(3.0);
    check(sameVector(mutable_v, 3.0, 6.0), "неконстантный scale(factor) меняет объект");
}

void testComparisons() {
    Vector2D a(3.0, 4.0);   // length 5
    Vector2D b(5.0, 0.0);   // length 5
    Vector2D c(1.0, 1.0);   // length ~1.41

    // сравнение идёт по длине
    check(a == b, "operator== сравнивает длины (5 == 5)");
    check(!(a == c), "operator== для разных длин == false");
    check(a != c, "operator!= для разных длин == true");
    check(c < a, "operator< по длине");
    check(a > c, "operator> по длине");
    check(a <= b, "operator<= при равных длинах");
    check(a >= b, "operator>= при равных длинах");
    check(!(c > a), "operator> false при меньшей длине");
}

void testArithmetic() {
    Vector2D a(1.0, 2.0);
    Vector2D b(3.0, 4.0);

    check(sameVector(a + b, 4.0, 6.0), "operator+");
    check(sameVector(b - a, 2.0, 2.0), "operator-");
    check(sameVector(a * 2.0, 2.0, 4.0), "operator* (вектор * скаляр)");
    check(sameVector(2.0 * a, 2.0, 4.0), "operator* (скаляр * вектор, friend)");
    check(sameVector(b / 2.0, 1.5, 2.0), "operator/");
    check(sameVector(-a, -1.0, -2.0), "унарный operator-");

    Vector2D c(1.0, 2.0);
    c += b;
    check(sameVector(c, 4.0, 6.0), "operator+=");
    c -= b;
    check(sameVector(c, 1.0, 2.0), "operator-=");

    check(throwsInvalidArgument([&] { a / 0.0; }),
          "деление на 0 -> std::invalid_argument");
}

} // namespace

int main() {
    testConstructorAndGetters();
    testScale();
    testComparisons();
    testArithmetic();

    std::cout << "\nИтого: " << g_checks << " проверок, "
              << g_failures << " провалено\n";

    return g_failures == 0 ? 0 : 1;
}
