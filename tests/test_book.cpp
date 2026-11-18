// Unit tests for Book, RareBook and Ebook (practise-5): виртуальные функции.
// Self-contained: no external test framework required.
// Build: g++ -std=c++17 -Wall -Wextra -pedantic book.cpp rarebook.cpp ebook.cpp tests/test_book.cpp -o run_tests

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

#include "../book.h"
#include "../ebook.h"
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
    check(book.pages() == 1225, "Book: pages()");
    check(book.isAvailable(), "Book: доступна по умолчанию");
    check(book.description() == "Война и мир by Л. Толстой (1869, 1225 pages)",
          "Book: description() формат");

    check(throwsInvalidArgument([] { Book b("", "Автор", 2000); }),
          "Book: пустое название -> std::invalid_argument");
    check(throwsInvalidArgument([] { Book b("Название", "Автор", 2026); }),
          "Book: год 2026 -> std::invalid_argument");
}

void testRareBook() {
    RareBook rare("Первое издание", "А. С. Пушкин", 1833, 200, 15000.0);

    check(rare.title() == "Первое издание", "RareBook: унаследованный title()");
    check(nearlyEqual(rare.estimatedValue(), 15000.0), "RareBook: estimatedValue()");

    rare.applyDiscount(10.0);
    check(nearlyEqual(rare.estimatedValue(), 13500.0), "RareBook: applyDiscount(10)");
    check(throwsInvalidArgument([&] { rare.applyDiscount(101.0); }),
          "RareBook: applyDiscount(101) -> std::invalid_argument");
    check(throwsInvalidArgument([] { RareBook r("Т", "А", 1900, 100, -1.0); }),
          "RareBook: отрицательная ценность -> std::invalid_argument");

    check(rare.description().find("[Estimated value: $") != std::string::npos,
          "RareBook: description() содержит ценность");
}

void testEbook() {
    Ebook ebook("Электронная книга", "Автор", 2020, 300, "https://example.com/book");

    check(ebook.title() == "Электронная книга", "Ebook: унаследованный title()");
    check(ebook.pages() == 300, "Ebook: унаследованный pages()");
    check(ebook.url() == "https://example.com/book", "Ebook: url()");

    check(ebook.description()
              == "Электронная книга by Автор (2020, 300 pages) [URL: https://example.com/book]",
          "Ebook: description() содержит URL");

    ebook.download();  // просто проверяем, что не падает
    check(true, "Ebook: download() выполняется без ошибок");

    check(throwsInvalidArgument([] { Ebook e("Т", "А", 1449, 100, "url"); }),
          "Ebook: невалидные данные Book -> std::invalid_argument");
}

void testPolymorphism() {
    Book plain("Обычная", "Автор", 2000, 100);
    RareBook rare("Редкая", "Автор", 1900, 100, 1000.0);
    Ebook ebook("Электронная", "Автор", 2020, 100, "https://example.com/e");

    // description() виртуальный: через ссылку на Book вызывается версия наследника
    Book& refRare = rare;
    check(refRare.description().find("[Estimated value: $") != std::string::npos,
          "полиморфизм: Book& -> RareBook::description()");

    Book& refEbook = ebook;
    check(refEbook.description().find("[URL: ") != std::string::npos,
          "полиморфизм: Book& -> Ebook::description()");

    Book& refPlain = plain;
    check(refPlain.description() == "Обычная by Автор (2000, 100 pages)",
          "полиморфизм: Book& -> Book::description()");

    Book* ptr = &ebook;
    check(ptr->description() == "Электронная by Автор (2020, 100 pages) [URL: https://example.com/e]",
          "полиморфизм: Book* -> Ebook::description()");
}

} // namespace

int main() {
    testBookBasics();
    testRareBook();
    testEbook();
    testPolymorphism();

    std::cout << "\nИтого: " << g_checks << " проверок, "
              << g_failures << " провалено\n";

    return g_failures == 0 ? 0 : 1;
}
