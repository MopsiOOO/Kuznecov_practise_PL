#include <iostream>
#include <stdexcept>

#include "book.h"
#include "rarebook.h"

int main() {
    Book b("Война и мир", "Л. Толстой", 1869, 1225);
    RareBook rare("Первое издание", "А. С. Пушкин", 1833, 200, 15000.0);

    std::cout << "Унаследованные геттеры RareBook:" << std::endl;
    std::cout << "title: " << rare.title() << std::endl;
    std::cout << "author: " << rare.author() << std::endl;
    std::cout << "year: " << rare.year() << std::endl;
    std::cout << "pages: " << rare.pages() << std::endl;

    rare.checkout();
    std::cout << "isAvailable после checkout: " << (rare.isAvailable() ? "true" : "false") << std::endl;
    rare.returnBook();
    std::cout << "isAvailable после returnBook: " << (rare.isAvailable() ? "true" : "false") << std::endl;

    std::cout << "Book description: " << b.description() << std::endl;
    std::cout << "RareBook description: " << rare.description() << std::endl;

    std::cout << "estimatedValue: " << rare.estimatedValue() << std::endl;
    rare.applyDiscount(10.0);
    std::cout << "estimatedValue после скидки 10%: " << rare.estimatedValue() << std::endl;
    std::cout << "RareBook description: " << rare.description() << std::endl;

    try {
        rare.applyDiscount(150.0);
        std::cout << "Ошибка: исключение не выброшено!" << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Не удалось применить скидку: " << e.what() << std::endl;
    }

    return 0;
}
