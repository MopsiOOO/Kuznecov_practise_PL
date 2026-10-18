#include "ebook.h"

#include <iostream>
#include <string>

Ebook::Ebook(std::string title, std::string author, int year, int pages,
             std::string url)
    : Book(title, author, year, pages), url_(std::move(url)) {
}
std::string Ebook::url() const {
    return url_;
}
void Ebook::download() const {
    std::cout << "Началась загрузка книги \"" << title() << "\" с " << url_ << std::endl;
}
std::string Ebook::description() const {
    return Book::description() + " [URL: " + url_ + "]";
}
