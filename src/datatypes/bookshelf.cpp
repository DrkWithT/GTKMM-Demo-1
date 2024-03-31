/**
 * @file bookshelf.cpp
 * @author DrkWithT
 * @brief Implements bookshelf dataclass.
 * @version 0.1
 * @date 2024-03-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <exception>
#include "datatypes/bookshelf.hpp"

namespace mynotehub::datatypes
{
    constexpr const char* placeholder_owner = "Anonymous";

    Bookshelf::Bookshelf()
    : titles {}, books {}, owner {placeholder_owner} {}

    Bookshelf::Bookshelf(const std::string& owner_str)
    : titles {}, books {}, owner {owner_str} {}

    const std::string& Bookshelf::getOwnerName() const
    {
        return owner;
    }

    const std::vector<Notebook>& Bookshelf::getAllBooks() const
    {
        return books;
    }

    const Notebook& Bookshelf::getNotebook(const std::string& title) const
    {
        for (const auto& book : books)
        {
            if (book.getTitle() == title)
            {
                return book;
            }
        }

        throw std::invalid_argument {"Bookshelf::getNotebook() Title not found."};
    }

    [[nodiscard]] bool Bookshelf::appendNotebook(Notebook book)
    {
        if (titles.find(book.getTitle()) != titles.end())
        {
            return false;
        }

        titles.insert(book.getTitle());
        books.emplace_back(std::move(book));

        return true;
    }

    [[nodiscard]] bool Bookshelf::removeNotebook(const std::string& title)
    {
        auto title_location = titles.find(title);

        if (title_location == titles.end())
        {
            return false;
        }

        auto book_location = books.begin();

        while (book_location != books.end())
        {
            if (book_location->getTitle() == title)
            {
                titles.erase(title_location);
                books.erase(book_location);
                break;
            }
        }

        return true;
    }
}
