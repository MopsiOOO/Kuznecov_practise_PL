// Unit tests for the Book class (practise-3): значения/ссылки/указатели.
// Self-contained: no external test framework required.
// Build: g++ -std=c++17 -Wall -Wextra -pedantic book.cpp tests/test_book.cpp -o run_tests

#include <cmath>
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

void testConstructorAndGetters() {
    Book book("Война и мир", "Л. Толстой", 1869, 1225);
    check(book.title() == "Война и мир", "getter title() возвращает название");
    check(book.author() == "Л. Толстой", "getter author() возвращает автора");
    check(book.year() == 1869, "getter year() возвращает год");
    check(book.pages() == 1225, "getter pages() возвращает число страниц");
    check(book.isAvailable(), "новая книга доступна по умолчанию");

    Book defaults("1984", "Д. Оруэлл", 1949);
    check(defaults.pages() == 100, "pages по умолчанию равно 100");
}

void testValidationAndUpdaters() {
    check(throwsInvalidArgument([] { Book b("", "Автор", 2000); }),
          "пустое название -> std::invalid_argument");
    check(throwsInvalidArgument([] { Book b("Название", "", 2000); }),
          "пустой автор -> std::invalid_argument");
    check(throwsInvalidArgument([] { Book b("Название", "Автор", 1449); }),
          "год 1449 -> std::invalid_argument");
    check(throwsInvalidArgument([] { Book b("Название", "Автор", 2026); }),
          "год 2026 -> std::invalid_argument");
    check(throwsInvalidArgument([] { Book b("Название", "Автор", 2000, 0); }),
          "0 страниц -> std::invalid_argument");

    Book book("1984", "Д. Оруэлл", 1949, 328);
    book.updateTitle("Nineteen Eighty-Four");
    book.updateAuthor("George Orwell");
    book.updateYear(1948);
    book.updatePages(320);
    check(book.title() == "Nineteen Eighty-Four" && book.author() == "George Orwell"
              && book.year() == 1948 && book.pages() == 320,
          "update*() обновляют поля");

    check(throwsInvalidArgument([&] { book.updateYear(3000); }),
          "updateYear(3000) -> std::invalid_argument");
    check(book.year() == 1948, "год не изменился после ошибки");
    check(throwsInvalidArgument([&] { book.updatePages(-50); }),
          "updatePages(-50) -> std::invalid_argument");
}

void testCheckoutAndReturn() {
    Book book("1984", "Д. Оруэлл", 1949, 328);
    book.checkout();
    check(!book.isAvailable(), "checkout() делает книгу недоступной");
    book.returnBook();
    check(book.isAvailable(), "returnBook() возвращает книгу в доступ");
}

void testFreeFunctions() {
    Book warAndPeace("Война и мир", "Л. Толстой", 1869, 1225);
    Book nineteenEightyFour("1984", "Д. Оруэлл", 1949, 328);

    check(formatBookInfo(nineteenEightyFour)
              == "Title: 1984, Author: Д. Оруэлл, Year: 1949, Pages: 328, Status: Available",
          "formatBookInfo() для доступной книги");

    check(isClassic(warAndPeace), "isClassic(1869) == true");
    check(isClassic(nineteenEightyFour), "isClassic(1949) == true (раньше 1975)");
    Book modern("Современная", "Автор", 2020);
    check(!isClassic(modern), "isClassic(2020) == false");

    check(nearlyEqual(readingTime(100, 50), 2.0), "readingTime(100, 50) == 2 дня");
    check(throwsInvalidArgument([] { readingTime(100, 0); }),
          "readingTime со скоростью 0 -> std::invalid_argument");

    check(isOlder(warAndPeace, nineteenEightyFour), "isOlder(1869, 1949) == true");
    check(!isOlder(nineteenEightyFour, warAndPeace), "isOlder(1949, 1869) == false");
}

void testByValue() {
    Book a("Первая", "Автор А", 2000);
    Book b("Вторая", "Автор Б", 2010);

    swap_titles_by_balue(a, b);
    check(a.title() == "Первая" && b.title() == "Вторая",
          "swap_titles_by_balue() не меняет оригиналы (передача по значению)");
}

void testByReference() {
    Book a("Первая", "Автор А", 2000);
    Book b("Вторая", "Автор Б", 2010);

    swap_titles_by_reference(a, b);
    check(a.title() == "Вторая" && b.title() == "Первая",
          "swap_titles_by_reference() меняет названия местами");

    Book older("Старая", "Автор", 1900);
    Book newer("Новая", "Автор", 2000);
    check(is_older(older, newer), "is_older(1900, 2000) == true");
    check(!is_older(newer, older), "is_older(2000, 1900) == false");
    check(!is_older(older, older), "is_older(одинаковые годы) == false");
}

void testByPointer() {
    Book a("Первая", "Автор А", 2000);
    Book b("Вторая", "Автор Б", 2010);

    swap_titles_by_pointer(&a, &b);
    check(a.title() == "Вторая" && b.title() == "Первая",
          "swap_titles_by_pointer() меняет названия местами");

    Book book("1984", "Д. Оруэлл", 1949, 328);
    Book* ptr = &book;
    check(is_book_available(ptr), "is_book_available() для доступной книги == true");
    book.checkout();
    check(!is_book_available(ptr), "is_book_available() после checkout == false");
    check(!is_book_available(nullptr), "is_book_available(nullptr) == false");
}

} // namespace

int main() {
    testConstructorAndGetters();
    testValidationAndUpdaters();
    testCheckoutAndReturn();
    testFreeFunctions();
    testByValue();
    testByReference();
    testByPointer();

    std::cout << "\nИтого: " << g_checks << " проверок, "
              << g_failures << " провалено\n";

    return g_failures == 0 ? 0 : 1;
}
