#ifndef BOOK_H
#define BOOK_H

#include <string>

class Book {
public:
    Book(std::string title, std::string author, int year);

    std::string title() const;
    std::string author() const;
    int year() const;
    bool isAvailable() const;

    void checkout();
    void returnBook();

    std::string getInfo() const;

private:
    bool isValidData(std::string title, std::string author, int year);

    std::string title_;
    std::string author_;
    int year_;
    bool is_available_;
};

#endif
