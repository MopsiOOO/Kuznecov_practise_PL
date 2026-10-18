#include <iostream>
#include <stdexcept>

#include "book.h"

int main() {
    Book warAndPeace("Война и мир", "Л. Толстой", 1869);
    Book nineteenEightyFour("1984", "Д. Оруэлл", 1949);

    std::cout << warAndPeace.getInfo() << std::endl;
    warAndPeace.checkout();
    std::cout << warAndPeace.getInfo() << std::endl;
    std::cout << nineteenEightyFour.getInfo() << std::endl;

    try {
        Book invalidYear("Некорректная книга", "А. Автор", 3000);
        std::cout << "Ошибка: исключение не выброшено!" << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Не удалось создать книгу: " << e.what() << std::endl;
    }
    try {
        Book emptyTitle("", "А. Автор", 2000);
        std::cout << "Ошибка: исключение не выброшено!" << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Не удалось создать книгу: " << e.what() << std::endl;
    }

    return 0;
}
