// Unit tests for Book and Library (practise-8): std::vector и композиция.
// Self-contained: no external test framework required.
// Build: g++ -std=c++17 -Wall -Wextra -pedantic book.cpp library.cpp tests/test_library.cpp -o run_tests

#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "../book.h"
#include "../library.h"

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
    check(book.description() == "Война и мир by Л. Толстой (1869, 1225 pages)",
          "Book: description() формат");

    check(throwsInvalidArgument([] { Book b("", "Автор", 2000); }),
          "Book: пустое название -> std::invalid_argument");
    check(throwsInvalidArgument([] { Book b("Название", "Автор", 2026); }),
          "Book: год 2026 -> std::invalid_argument");
    check(throwsInvalidArgument([] { Book b("Название", "Автор", 2000, 0); }),
          "Book: 0 страниц -> std::invalid_argument");
}

void testBookDefaultCtor() {
    // Конструктор по умолчанию нужен Library для работы с контейнерами;
    // поля при этом не инициализированы, поэтому сразу присваиваем валидную книгу.
    Book b;
    b = Book("1984", "Д. Оруэлл", 1949, 328);
    check(b.title() == "1984" && b.year() == 1949 && b.pages() == 328,
          "Book: конструктор по умолчанию + присваивание");
}

void testLibraryAddAndSize() {
    Library lib;
    check(lib.size() == 0, "Library: новая библиотека пуста");

    lib.add(Book("Война и мир", "Л. Толстой", 1869, 1225));
    check(lib.size() == 1, "Library: add() увеличивает размер");

    lib.add(Book("1984", "Д. Оруэлл", 1949, 328));
    lib.add(Book("Евгений Онегин", "А. Пушкин", 1833, 300));
    check(lib.size() == 3, "Library: три книги -> size() == 3");
}

void testLibraryRemove() {
    Library lib;
    lib.add(Book("Первая", "Автор", 2000));
    lib.add(Book("Вторая", "Автор", 2001));
    lib.add(Book("Третья", "Автор", 2002));

    lib.remove(1);
    check(lib.size() == 2, "Library: remove(1) удаляет книгу");

    std::ostringstream oss;
    oss << lib;
    check(oss.str().find("Первая") != std::string::npos
              && oss.str().find("Третья") != std::string::npos
              && oss.str().find("Вторая") == std::string::npos,
          "Library: после remove(1) остались Первая и Третья");

    lib.remove(-1);
    check(lib.size() == 2, "Library: remove(-1) ничего не удаляет");
    lib.remove(99);
    check(lib.size() == 2, "Library: remove(99) ничего не удаляет");
    lib.remove(0);
    lib.remove(0);
    check(lib.size() == 0, "Library: удаление всех книг -> size() == 0");
    lib.remove(0);
    check(lib.size() == 0, "Library: remove() из пустой библиотеки безопасен");
}

void testLibraryOutput() {
    Library lib;
    std::ostringstream empty;
    empty << lib;
    check(empty.str().empty(), "Library: пустая библиотека -> пустой вывод");

    lib.add(Book("Война и мир", "Л. Толстой", 1869, 1225));
    lib.add(Book("1984", "Д. Оруэлл", 1949, 328));

    std::ostringstream oss;
    oss << lib;
    const std::string expected =
        "Война и мир by Л. Толстой (1869, 1225 pages)\n"
        "1984 by Д. Оруэлл (1949, 328 pages)\n";
    check(oss.str() == expected, "Library: operator<< выводит описания книг построчно");
}

} // namespace

int main() {
    testBookBasics();
    testBookDefaultCtor();
    testLibraryAddAndSize();
    testLibraryRemove();
    testLibraryOutput();

    std::cout << "\nИтого: " << g_checks << " проверок, "
              << g_failures << " провалено\n";

    return g_failures == 0 ? 0 : 1;
}
