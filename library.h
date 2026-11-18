#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include <iostream>
#include "book.h"

// std::vector<Book> выбран вместо Book books_[1000] и std::array<Book, 1000>,
// потому что:
//  - C-style массив фиксированного размера тратит память впустую (1000 слотов
//    при 4 книгах) и не знает своего размера — его пришлось бы передавать вручную;
//  - std::array также фиксирован: место под 1000 книг выделяется всегда,
//    даже если книг мало, и размер нельзя изменить после создания;
//  - std::vector растёт и сжимается по мере add/remove, хранит свой размер
//    и поддерживает проверку границ — именно то, что нужно библиотеке.
class Library {
public:
    Library();
    void add(const Book& book);
    void remove(int id);
    size_t size() const;
    friend std::ostream& operator<<(std::ostream& os, const Library& lib);
private:
    std::vector<Book> books_;
};

#endif // LIBRARY_H
