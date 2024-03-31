#ifndef BOOKSHELF_HPP
#define BOOKSHELF_HPP

#include <vector>
#include <set>
#include "datatypes/notebook.hpp"

namespace mynotehub::datatypes
{
    class Bookshelf
    {
    private:
        std::set<std::string> titles;
        std::vector<Notebook> books;
        std::string owner;

    public:
        Bookshelf();
        Bookshelf(const std::string& owner_str);

        const std::string& getOwnerName() const;

        const std::vector<Notebook>& getAllBooks() const;
        const Notebook& getNotebook(const std::string& title) const;
        [[nodiscard]] bool appendNotebook(Notebook book);
        [[nodiscard]] bool removeNotebook(const std::string& title);
    };
}

#endif