// Unit tests for the Book class (practise-2).
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

void testValidation() {
    check(throwsInvalidArgument([] { Book b("", "Автор", 2000); }),
          "пустое название -> std::invalid_argument");
    check(throwsInvalidArgument([] { Book b("Название", "", 2000); }),
          "пустой автор -> std::invalid_argument");
    check(throwsInvalidArgument([] { Book b("Название", "Автор", 1449); }),
          "год 1449 (раньше 1450) -> std::invalid_argument");
    check(throwsInvalidArgument([] { Book b("Название", "Автор", 2026); }),
          "год 2026 (позже 2025) -> std::invalid_argument");
    check(throwsInvalidArgument([] { Book b("Название", "Автор", 2000, 0); }),
          "0 страниц -> std::invalid_argument");
    check(throwsInvalidArgument([] { Book b("Название", "Автор", 2000, -5); }),
          "отрицательное число страниц -> std::invalid_argument");

    bool boundaryOk = true;
    try {
        Book oldest("Старая", "Автор", 1450, 1);
        Book newest("Новая", "Автор", 2025, 1);
        boundaryOk = (oldest.year() == 1450) && (newest.year() == 2025);
    } catch (...) {
        boundaryOk = false;
    }
    check(boundaryOk, "граничные годы 1450 и 2025 принимаются");
}

void testUpdaters() {
    Book book("1984", "Д. Оруэлл", 1949, 328);

    book.updateTitle("Nineteen Eighty-Four");
    check(book.title() == "Nineteen Eighty-Four", "updateTitle() обновляет название");
    book.updateAuthor("George Orwell");
    check(book.author() == "George Orwell", "updateAuthor() обновляет автора");
    book.updateYear(1948);
    check(book.year() == 1948, "updateYear() обновляет год");
    book.updatePages(320);
    check(book.pages() == 320, "updatePages() обновляет страницы");

    check(throwsInvalidArgument([&] { book.updateTitle(""); }),
          "updateTitle(пусто) -> std::invalid_argument");
    check(book.title() == "Nineteen Eighty-Four", "название не изменилось после ошибки");
    check(throwsInvalidArgument([&] { book.updateAuthor(""); }),
          "updateAuthor(пусто) -> std::invalid_argument");
    check(throwsInvalidArgument([&] { book.updateYear(3000); }),
          "updateYear(3000) -> std::invalid_argument");
    check(book.year() == 1948, "год не изменился после ошибки");
    check(throwsInvalidArgument([&] { book.updatePages(-50); }),
          "updatePages(-50) -> std::invalid_argument");
    check(book.pages() == 320, "страницы не изменились после ошибки");
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
    warAndPeace.checkout();
    check(formatBookInfo(warAndPeace)
              == "Title: Война и мир, Author: Л. Толстой, Year: 1869, Pages: 1225, Status: Checked Out",
          "formatBookInfo() для выданной книги");

    Book classic("Евгений Онегин", "А. Пушкин", 1833, 300);
    Book modern("Современная", "Автор", 2020);
    Book borderline("Рубеж", "Автор", 1975);
    check(isClassic(classic), "isClassic(1833) == true");
    check(!isClassic(borderline), "isClassic(1975) == false (строго меньше 1975)");
    check(!isClassic(modern), "isClassic(2020) == false");

    check(nearlyEqual(readingTime(100, 50), 2.0), "readingTime(100, 50) == 2 дня");
    check(nearlyEqual(readingTime(1225, 50), 24.5), "readingTime(1225, 50) == 24.5 дня");
    check(throwsInvalidArgument([] { readingTime(100, 0); }),
          "readingTime со скоростью 0 -> std::invalid_argument");
    check(throwsInvalidArgument([] { readingTime(100, -10); }),
          "readingTime с отрицательной скоростью -> std::invalid_argument");

    check(isOlder(warAndPeace, nineteenEightyFour), "isOlder(1869, 1949) == true");
    check(!isOlder(nineteenEightyFour, warAndPeace), "isOlder(1949, 1869) == false");
    check(!isOlder(nineteenEightyFour, nineteenEightyFour), "isOlder(одинаковые годы) == false");
}

} // namespace

int main() {
    testConstructorAndGetters();
    testValidation();
    testUpdaters();
    testCheckoutAndReturn();
    testFreeFunctions();

    std::cout << "\nИтого: " << g_checks << " проверок, "
              << g_failures << " провалено\n";

    return g_failures == 0 ? 0 : 1;
}
