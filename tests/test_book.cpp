// Unit tests for the Book class (practise-3, v2): передача по значению,
// по ссылке, const-ссылки и указатели.
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

// ---------------------------------------------------------------- Базовый Book

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
    check(throwsInvalidArgument([] { Book b("", "", 2000); }),
          "пустые название и автор -> std::invalid_argument");
    check(throwsInvalidArgument([] { Book b("Название", "Автор", 1449); }),
          "год 1449 (раньше 1450) -> std::invalid_argument");
    check(throwsInvalidArgument([] { Book b("Название", "Автор", 2026); }),
          "год 2026 (позже 2025) -> std::invalid_argument");
    check(throwsInvalidArgument([] { Book b("Название", "Автор", 2000, 0); }),
          "0 страниц -> std::invalid_argument");
    check(throwsInvalidArgument([] { Book b("Название", "Автор", 2000, -10); }),
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
    book.updateAuthor("George Orwell");
    book.updateYear(1948);
    book.updatePages(320);
    check(book.title() == "Nineteen Eighty-Four" && book.author() == "George Orwell"
              && book.year() == 1948 && book.pages() == 320,
          "update*() обновляют все поля");

    check(throwsInvalidArgument([&] { book.updateTitle(""); }),
          "updateTitle(пусто) -> std::invalid_argument");
    check(throwsInvalidArgument([&] { book.updateAuthor(""); }),
          "updateAuthor(пусто) -> std::invalid_argument");
    check(throwsInvalidArgument([&] { book.updateYear(3000); }),
          "updateYear(3000) -> std::invalid_argument");
    check(throwsInvalidArgument([&] { book.updatePages(-50); }),
          "updatePages(-50) -> std::invalid_argument");

    // неудачные обновления не должны менять состояние
    check(book.title() == "Nineteen Eighty-Four" && book.author() == "George Orwell"
              && book.year() == 1948 && book.pages() == 320,
          "состояние не меняется после неудачных update*()");
}

void testCheckoutAndReturn() {
    Book book("1984", "Д. Оруэлл", 1949, 328);
    book.checkout();
    check(!book.isAvailable(), "checkout() делает книгу недоступной");
    book.returnBook();
    check(book.isAvailable(), "returnBook() возвращает книгу в доступ");
}

// -------------------------------------------------------- Вспомогательные функции

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
    warAndPeace.returnBook();

    check(isClassic(warAndPeace), "isClassic(1869) == true");
    check(isClassic(nineteenEightyFour), "isClassic(1949) == true (раньше 1975)");
    Book modern("Современная", "Автор", 2020);
    Book borderline("Рубеж", "Автор", 1975);
    check(!isClassic(modern), "isClassic(2020) == false");
    check(!isClassic(borderline), "isClassic(1975) == false (строго меньше 1975)");

    check(nearlyEqual(readingTime(100, 50), 2.0), "readingTime(100, 50) == 2 дня");
    check(nearlyEqual(readingTime(1225, 50), 24.5), "readingTime(1225, 50) == 24.5 дня");
    check(throwsInvalidArgument([] { readingTime(100, 0); }),
          "readingTime со скоростью 0 -> std::invalid_argument");
    check(throwsInvalidArgument([] { readingTime(100, -10); }),
          "readingTime с отрицательной скоростью -> std::invalid_argument");

    check(isOlder(warAndPeace, nineteenEightyFour), "isOlder(1869, 1949) == true");
    check(!isOlder(nineteenEightyFour, warAndPeace), "isOlder(1949, 1869) == false");
}

// -------------------------------------- Часть 1. Передача по значению (копия)

void testPassByValue() {
    Book a("Первая", "Автор А", 2000);
    Book b("Вторая", "Автор Б", 2010);

    swap_titles_by_balue(a, b);
    check(a.title() == "Первая", "swap_titles_by_balue(): первая книга не изменилась");
    check(b.title() == "Вторая", "swap_titles_by_balue(): вторая книга не изменилась");
}

// ------------------------------------------ Часть 2. Передача по ссылке

void testPassByReference() {
    Book a("Первая", "Автор А", 2000);
    Book b("Вторая", "Автор Б", 2010);

    swap_titles_by_reference(a, b);
    check(a.title() == "Вторая", "swap_titles_by_reference(): первая книга получила название второй");
    check(b.title() == "Первая", "swap_titles_by_reference(): вторая книга получила название первой");

    // повторный вызов возвращает названия обратно
    swap_titles_by_reference(a, b);
    check(a.title() == "Первая" && b.title() == "Вторая",
          "swap_titles_by_reference(): повторный вызов меняет названия обратно");

    // обмен книги с самой собой не ломает её
    swap_titles_by_reference(a, a);
    check(a.title() == "Первая", "swap_titles_by_reference(): обмен с самой собой безопасен");
}

// ------------------------------------------ Часть 3. Const-ссылки

void testConstReference() {
    Book older("Старая", "Автор", 1900);
    Book newer("Новая", "Автор", 2000);

    check(is_older(older, newer), "is_older(1900, 2000) == true");
    check(!is_older(newer, older), "is_older(2000, 1900) == false");
    check(!is_older(older, older), "is_older(одинаковые годы) == false");

    // const-ссылка не должна менять переданные книги
    check(older.title() == "Старая" && older.year() == 1900
              && newer.title() == "Новая" && newer.year() == 2000,
          "is_older() не изменяет аргументы (const-ссылки)");
}

// ------------------------------------------ Часть 4. Указатели

void testPassByPointer() {
    Book a("Первая", "Автор А", 2000);
    Book b("Вторая", "Автор Б", 2010);

    swap_titles_by_pointer(&a, &b);
    check(a.title() == "Вторая", "swap_titles_by_pointer(): первая книга получила название второй");
    check(b.title() == "Первая", "swap_titles_by_pointer(): вторая книга получила название первой");

    // повторный вызов возвращает названия обратно
    swap_titles_by_pointer(&a, &b);
    check(a.title() == "Первая" && b.title() == "Вторая",
          "swap_titles_by_pointer(): повторный вызов меняет названия обратно");

    // обмен книги с самой собой не ломает её
    swap_titles_by_pointer(&a, &a);
    check(a.title() == "Первая", "swap_titles_by_pointer(): обмен с самой собой безопасен");
}

// ------------------------------------------ Часть 5. Доступность через указатель

void testAvailabilityByPointer() {
    Book book("1984", "Д. Оруэлл", 1949, 328);
    Book* ptr = &book;

    check(is_book_available(ptr), "is_book_available(): доступная книга -> true");
    book.checkout();
    check(!is_book_available(ptr), "is_book_available(): после checkout -> false");
    book.returnBook();
    check(is_book_available(ptr), "is_book_available(): после returnBook -> true");
    check(!is_book_available(nullptr), "is_book_available(nullptr) -> false");
}

} // namespace

int main() {
    testConstructorAndGetters();
    testValidation();
    testUpdaters();
    testCheckoutAndReturn();
    testFreeFunctions();
    testPassByValue();
    testPassByReference();
    testConstReference();
    testPassByPointer();
    testAvailabilityByPointer();

    std::cout << "\nИтого: " << g_checks << " проверок, "
              << g_failures << " провалено\n";

    return g_failures == 0 ? 0 : 1;
}
