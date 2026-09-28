#include <iostream>

#include "book.h"

int main() {
    Book warAndPeace("Война и мир", "Л. Толстой", 1869, 1225);
    Book nineteenEightyFour("1984", "Джордж Оруэлл", 1949, 328);

    std::cout << "Часть 1. Передача по значению" << std::endl;
    std::cout << "До: " << warAndPeace.title() << " | " << nineteenEightyFour.title() << std::endl;
    swap_titles_by_balue(warAndPeace, nineteenEightyFour);
    std::cout << "После: " << warAndPeace.title() << " | " << nineteenEightyFour.title() << std::endl;

    std::cout << "Часть 2. Передача по ссылке" << std::endl;
    std::cout << "До: " << warAndPeace.title() << " | " << nineteenEightyFour.title() << std::endl;
    swap_titles_by_reference(warAndPeace, nineteenEightyFour);
    std::cout << "После: " << warAndPeace.title() << " | " << nineteenEightyFour.title() << std::endl;

    std::cout << "Часть 3. Const-ссылки" << std::endl;
    std::cout << "\"" << warAndPeace.title() << "\" старше \"" << nineteenEightyFour.title()
              << "\": " << (is_older(warAndPeace, nineteenEightyFour) ? "да" : "нет") << std::endl;

    std::cout << "Часть 4. Указатели" << std::endl;
    std::cout << "До: " << warAndPeace.title() << " | " << nineteenEightyFour.title() << std::endl;
    swap_titles_by_pointer(&warAndPeace, &nineteenEightyFour);
    std::cout << "После: " << warAndPeace.title() << " | " << nineteenEightyFour.title() << std::endl;

    std::cout << "Часть 5. Проверка доступности" << std::endl;
    Book* ptr = &warAndPeace;
    std::cout << "Книга доступна: " << (is_book_available(ptr) ? "да" : "нет") << std::endl;
    warAndPeace.checkout();
    std::cout << "Книга доступна после checkout: " << (is_book_available(ptr) ? "да" : "нет") << std::endl;
    std::cout << "nullptr доступен: " << (is_book_available(nullptr) ? "да" : "нет") << std::endl;

    return 0;
}
