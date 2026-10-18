#include "book.h"

#include <ctime>
#include <stdexcept>

Book::Book(std::string title, std::string author, int year)
    : title_(std::move(title))
    , author_(std::move(author))
    , year_(year)
    , is_available_(true)
{
    if (!isValidData(title_, author_, year_)) {
        throw std::invalid_argument("Invalid book data: title and author must be non-empty, "
                                    "year must be between 1450 and the current year.");
    }
}

std::string Book::title() const {
    return title_;
}
std::string Book::author() const {
    return author_;
}
int Book::year() const {
    return year_;
}
bool Book::isAvailable() const {
    return is_available_;
}

void Book::checkout() {
    is_available_ = false;
}
void Book::returnBook() {
    is_available_ = true;
}

std::string Book::getInfo() const {
    return "Title: " + title_
        + ", Author: " + author_
        + ", Year: " + std::to_string(year_)
        + ", Status: " + (is_available_ ? "Available" : "Checked Out");
}

bool Book::isValidData(std::string title, std::string author, int year) {
    if (title.empty() || author.empty()) {
        return false;
    }
    const std::time_t now = std::time(nullptr);
    const int current_year = std::localtime(&now)->tm_year + 1900;
    return year >= 1450 && year <= current_year;
}
