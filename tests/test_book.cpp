// Unit tests for the Book class.
// Self-contained: no external test framework required.
// Build: g++ -std=c++17 -Wall -Wextra -pedantic book.cpp tests/test_book.cpp -o book_tests

#include <ctime>
#include <iostream>
#include <stdexcept>
#include <string>

#include "../book.h"

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

int currentYear() {
    const std::time_t now = std::time(nullptr);
    return std::localtime(&now)->tm_year + 1900;
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
    Book book("Война и мир", "Л. Толстой", 1869);
    check(book.title() == "Война и мир", "getter title() возвращает название");
    check(book.author() == "Л. Толстой", "getter author() возвращает автора");
    check(book.year() == 1869, "getter year() возвращает год");
    check(book.isAvailable(), "новая книга доступна по умолчанию");
}

void testCheckoutAndReturn() {
    Book book("1984", "Д. Оруэлл", 1949);

    book.checkout();
    check(!book.isAvailable(), "checkout() делает книгу недоступной");

    book.returnBook();
    check(book.isAvailable(), "returnBook() возвращает книгу в доступ");

    // Повторные вызовы не должны ломать состояние
    book.returnBook();
    check(book.isAvailable(), "повторный returnBook() оставляет книгу доступной");

    book.checkout();
    book.checkout();
    check(!book.isAvailable(), "повторный checkout() оставляет книгу недоступной");
}

void testGetInfo() {
    Book available("1984", "Д. Оруэлл", 1949);
    check(available.getInfo() == "Title: 1984, Author: Д. Оруэлл, Year: 1949, Status: Available",
          "getInfo() для доступной книги");

    Book checkedOut("Война и мир", "Л. Толстой", 1869);
    checkedOut.checkout();
    check(checkedOut.getInfo() == "Title: Война и мир, Author: Л. Толстой, Year: 1869, Status: Checked Out",
          "getInfo() для выданной книги");
}

void testValidation() {
    check(throwsInvalidArgument([] { Book b("", "Автор", 2000); }),
          "пустое название -> std::invalid_argument");
    check(throwsInvalidArgument([] { Book b("Название", "", 2000); }),
          "пустой автор -> std::invalid_argument");
    check(throwsInvalidArgument([] { Book b("", "", 2000); }),
          "пустые название и автор -> std::invalid_argument");
    check(throwsInvalidArgument([] { Book b("Название", "Автор", 1449); }),
          "год 1449 (раньше 1450) -> std::invalid_argument");
    check(throwsInvalidArgument([] { Book b("Название", "Автор", currentYear() + 1); }),
          "год из будущего -> std::invalid_argument");
    check(throwsInvalidArgument([] { Book b("Название", "Автор", -100); }),
          "отрицательный год -> std::invalid_argument");
}

void testBoundaryYears() {
    bool ok1450 = true;
    try {
        Book b("Гутенберг", "И. Гутенберг", 1450);
        ok1450 = (b.year() == 1450);
    } catch (...) {
        ok1450 = false;
    }
    check(ok1450, "год 1450 (нижняя граница) принимается");

    bool okNow = true;
    try {
        Book b("Современная книга", "Автор", currentYear());
        okNow = (b.year() == currentYear());
    } catch (...) {
        okNow = false;
    }
    check(okNow, "текущий год (верхняя граница) принимается");
}

} // namespace

int main() {
    testConstructorAndGetters();
    testCheckoutAndReturn();
    testGetInfo();
    testValidation();
    testBoundaryYears();

    std::cout << "\nИтого: " << g_checks << " проверок, "
              << g_failures << " провалено\n";

    return g_failures == 0 ? 0 : 1;
}
