#include "library.h"

Library::Library() {
}
void Library::add(const Book& book) {
    books_.push_back(book);
}
void Library::remove(int id) {
    if (id >= 0 && id < static_cast<int>(books_.size())) {
        books_.erase(books_.begin() + id);
    }
}
size_t Library::size() const {
    return books_.size();
}
std::ostream& operator<<(std::ostream& os, const Library& lib) {
    for (const Book& book : lib.books_) {
        os << book.description() << "\n";
    }
    return os;
}
