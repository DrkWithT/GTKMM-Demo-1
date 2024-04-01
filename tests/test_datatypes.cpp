/**
 * @file test_datatypes.cpp
 * @author DrkWithT
 * @brief Implements unit test suite for note datatypes.
 * @version 0.1
 * @date 2024-03-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "datatypes/notebook.hpp"
#include "datatypes/bookshelf.hpp"

using MySubject = mynotehub::datatypes::SubjectType;
using MyNote = mynotehub::datatypes::NotePage;
using MyBook = mynotehub::datatypes::Notebook;
using MyShelf = mynotehub::datatypes::Bookshelf;

int main()
{
    MyNote foo {"Note 1", "hello world!", 1};
    MyNote bar {"Note 2", "hola el mundo!", 1};
    MyBook book {"Test 1", MySubject::other};
    MyBook book2 {"Test 2", MySubject::other};
    MyShelf shelf {"Bob Jones"};

    book.appendPage(foo);

    if (book.getPageCount() != 1)
        return 1;

    if (!book.removePage(1))
        return 1;

    book.clearPages();

    if (book.getPageCount() != 0)
        return 1;

    book2.appendPage(bar);

    if (!shelf.appendNotebook(book2))
        return 1;

    if (shelf.getAllBooks().size() != 1)
        return 1;

    if (!shelf.removeNotebook("Test 2"))
        return 1;

    if (shelf.getAllBooks().size() != 0)
        return 1;
}
