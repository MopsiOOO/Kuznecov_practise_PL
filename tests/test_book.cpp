// Unit tests for Book and RareBook (practise-4): наследование.
// Self-contained: no external test framework required.
// Build: g++ -std=c++17 -Wall -Wextra -pedantic book.cpp rarebook.cpp tests/test_book.cpp -o run_tests

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

#include "../book.h"
#include "../rarebook.h"

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

void testBookBasics() {
    Book book("Война и мир", "Л. Толстой", 1869, 1225);
    check(book.title() == "Война и мир", "Book: title()");
    check(book.author() == "Л. Толстой", "Book: author()");
    check(book.year() == 1869, "Book: year()");
    check(book.pages() == 1225, "Book: pages()");
    check(book.isAvailable(), "Book: доступна по умолчанию");

    book.checkout();
    check(!book.isAvailable(), "Book: checkout() убирает из доступа");
    book.returnBook();
    check(book.isAvailable(), "Book: returnBook() возвращает в доступ");

    check(book.description() == "Война и мир by Л. Толстой (1869, 1225 pages)",
          "Book: description() формат");
}

void testBookValidation() {
    check(throwsInvalidArgument([] { Book b("", "Автор", 2000); }),
          "Book: пустое название -> std::invalid_argument");
    check(throwsInvalidArgument([] { Book b("Название", "Автор", 1449); }),
          "Book: год 1449 -> std::invalid_argument");
    check(throwsInvalidArgument([] { Book b("Название", "Автор", 2000, 0); }),
          "Book: 0 страниц -> std::invalid_argument");
}

void testRareBookInheritance() {
    RareBook rare("Первое издание", "А. С. Пушкин", 1833, 200, 15000.0);

    check(rare.title() == "Первое издание", "RareBook: унаследованный title()");
    check(rare.author() == "А. С. Пушкин", "RareBook: унаследованный author()");
    check(rare.year() == 1833, "RareBook: унаследованный year()");
    check(rare.pages() == 200, "RareBook: унаследованный pages()");
    check(rare.isAvailable(), "RareBook: доступна по умолчанию");

    rare.checkout();
    check(!rare.isAvailable(), "RareBook: унаследованный checkout()");
    rare.returnBook();
    check(rare.isAvailable(), "RareBook: унаследованный returnBook()");

    check(nearlyEqual(rare.estimatedValue(), 15000.0),
          "RareBook: estimatedValue()");
}

void testRareBookValidation() {
    check(throwsInvalidArgument([] { RareBook r("Т", "А", 1900, 100, -1.0); }),
          "RareBook: отрицательная ценность -> std::invalid_argument");
    check(throwsInvalidArgument([] { RareBook r("", "А", 1900, 100, 10.0); }),
          "RareBook: невалидные данные Book -> std::invalid_argument");

    bool zeroOk = true;
    try {
        RareBook free("Бесценная", "Автор", 1900, 100, 0.0);
        zeroOk = nearlyEqual(free.estimatedValue(), 0.0);
    } catch (...) {
        zeroOk = false;
    }
    check(zeroOk, "RareBook: нулевая ценность принимается");
}

void testApplyDiscount() {
    RareBook rare("Первое издание", "А. С. Пушкин", 1833, 200, 15000.0);

    rare.applyDiscount(10.0);
    check(nearlyEqual(rare.estimatedValue(), 13500.0),
          "applyDiscount(10) снижает ценность на 10%");

    rare.applyDiscount(100.0);
    check(nearlyEqual(rare.estimatedValue(), 0.0),
          "applyDiscount(100) обнуляет ценность");

    check(throwsInvalidArgument([&] { rare.applyDiscount(-1.0); }),
          "applyDiscount(-1) -> std::invalid_argument");
    check(throwsInvalidArgument([&] { rare.applyDiscount(100.1); }),
          "applyDiscount(100.1) -> std::invalid_argument");

    RareBook stable("Книга", "Автор", 1900, 100, 500.0);
    try {
        stable.applyDiscount(150.0);
    } catch (const std::invalid_argument&) {
    }
    check(nearlyEqual(stable.estimatedValue(), 500.0),
          "ценность не меняется после неудачного applyDiscount");
}

void testRareBookDescription() {
    RareBook rare("Первое издание", "А. С. Пушкин", 1833, 200, 15000.0);
    check(rare.description()
              == "Первое издание by А. С. Пушкин (1833, 200 pages) [Estimated value: $15000.000000]",
          "RareBook: description() расширяет описание Book");

    // В practise-4 description() не виртуальный: через ссылку на Book
    // вызывается Book::description()
    Book& asBook = rare;
    check(asBook.description() == "Первое издание by А. С. Пушкин (1833, 200 pages)",
          "через Book& вызывается Book::description() (не virtual)");
}

} // namespace

int main() {
    testBookBasics();
    testBookValidation();
    testRareBookInheritance();
    testRareBookValidation();
    testApplyDiscount();
    testRareBookDescription();

    std::cout << "\nИтого: " << g_checks << " проверок, "
              << g_failures << " провалено\n";

    return g_failures == 0 ? 0 : 1;
}
