#include <iostream>

#include "book.h"
#include "ebook.h"

void print_description(const Book& book) {
    std::cout << book.description() << std::endl;
}

int main() {
    Book b("1984", "George Orwell", 1949, 328);
    Ebook eb("1984", "George Orwell", 1949, 328, "http://example.com/1984");

    print_description(b);
    print_description(eb);

    return 0;
}
