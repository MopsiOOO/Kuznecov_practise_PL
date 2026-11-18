#include <array>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "book.h"
#include "library.h"

double average_pages(Book books[], int size) {
    double sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += books[i].pages();
    }
    return sum / size;
}
double average_pages(const std::array<Book, 5>& books) {
    double sum = 0;
    for (const Book& b : books) {
        sum += b.pages();
    }
    return sum / books.size();
}

int main() {
    Book books[5] = {
        Book("War and Peace", "Leo Tolstoy", 1869, 1225),
        Book("1984", "George Orwell", 1949, 328),
        Book("The Hobbit", "J.R.R. Tolkien", 1937, 310),
        Book("Dune", "Frank Herbert", 1965, 412),
        Book("Foundation", "Isaac Asimov", 1951, 255)
    };
    for (int i = 0; i < 5; ++i) {
        std::cout << books[i].description() << std::endl;
    }
    // books[7] = Book("Fake", "Nobody", 2000, 100); // ОПАСНО: для C-style
    // массива это не ошибка компиляции и обычно не падение, а запись по
    // произвольному адресу за границей массива (UB). Программа может
    // "работать", тихо портя чужие данные. std::array решает это методом
    // .at(), который бросает std::out_of_range, а также знает свой size().
    // size передаём вручную, потому что при передаче C-style массива в
    // функцию он «сворачивается» в указатель и информация о размере теряется.
    // Если забыть передать size или передать неверный, функция прочитает
    // мусор за границей массива.
    std::cout << "average pages (C array): " << average_pages(books, 5) << std::endl;

    std::array<Book, 5> arr_books = {
        Book("War and Peace", "Leo Tolstoy", 1869, 1225),
        Book("1984", "George Orwell", 1949, 328),
        Book("The Hobbit", "J.R.R. Tolkien", 1937, 310),
        Book("Dune", "Frank Herbert", 1965, 412),
        Book("Foundation", "Isaac Asimov", 1951, 255)
    };
    for (size_t i = 0; i < arr_books.size(); ++i) {
        std::cout << arr_books[i].description() << std::endl;
    }
    for (const auto& b : arr_books) {
        std::cout << b.description() << std::endl;
    }
    try {
        arr_books.at(7) = Book("Fake", "Nobody", 2000, 100);
    } catch (const std::out_of_range& e) {
        std::cerr << "Поймано исключение: " << e.what() << std::endl;
    }
    // .at() проверяет индекс и бросает std::out_of_range при выходе за
    // границы, а operator[] работает без проверки, как у C-style массива
    // (тоже UB при неверном индексе). Поэтому .at() безопаснее.
    std::cout << "average pages (std::array): " << average_pages(arr_books) << std::endl;

    std::vector<Book> vec_books = {
        Book("War and Peace", "Leo Tolstoy", 1869, 1225),
        Book("1984", "George Orwell", 1949, 328),
        Book("The Hobbit", "J.R.R. Tolkien", 1937, 310)
    };
    std::cout << "size до push_back: " << vec_books.size() << std::endl;
    vec_books.push_back(Book("Dune", "Frank Herbert", 1965, 412));
    vec_books.push_back(Book("Foundation", "Isaac Asimov", 1951, 255));
    std::cout << "size после push_back: " << vec_books.size() << std::endl;

    std::cout << "size до pop_back: " << vec_books.size() << std::endl;
    vec_books.pop_back();
    std::cout << "size после pop_back: " << vec_books.size() << std::endl;

    vec_books.erase(vec_books.begin() + 1);
    for (const Book& b : vec_books) {
        std::cout << b.description() << std::endl;
    }

    vec_books.resize(5, Book("Unknown", "Unknown", 2000, 100));
    std::cout << "size после resize(5): " << vec_books.size() << std::endl;
    for (const Book& b : vec_books) {
        std::cout << b.description() << std::endl;
    }
    vec_books.resize(2);
    std::cout << "size после resize(2): " << vec_books.size() << std::endl;
    for (const Book& b : vec_books) {
        std::cout << b.description() << std::endl;
    }

    Library lib;
    lib.add(Book("War and Peace", "Leo Tolstoy", 1869, 1225));
    lib.add(Book("1984", "George Orwell", 1949, 328));
    lib.add(Book("The Hobbit", "J.R.R. Tolkien", 1937, 310));
    lib.add(Book("Dune", "Frank Herbert", 1965, 412));
    std::cout << lib;
    lib.remove(1);
    std::cout << lib;
    std::cout << "lib.size() = " << lib.size() << std::endl;

    return 0;
}
