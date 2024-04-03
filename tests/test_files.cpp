/**
 * @file test_files.cpp
 * @author DrkWithT
 * @brief Implements unit test for save-file writer/loader.
 * @version 0.1
 * @date 2024-04-02
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "datatypes/notebook.hpp"
#include "datatypes/bookshelf.hpp"
#include "utils/files.hpp"

using MyPage = mynotehub::datatypes::NotePage;
using MyBook = mynotehub::datatypes::Notebook;
using MySubject = mynotehub::datatypes::SubjectType;
using MyShelf = mynotehub::datatypes::Bookshelf;
using MySaveWriter = mynotehub::utils::SaveMachine;
using MySaveLoader = mynotehub::utils::LoadMachine;
using SaveStatus = mynotehub::utils::SavingStatus;

const char* demo_owner = "Bob Jones";
const char* demo_book_title_1 = "Test 1";
const char* demo_page_title_1 = "Page 1";
const char* demo_content_1 = "Hello world, blah blah!";
const char* demo_save_path = "./testsaves/foo.dat";

int main()
{
    MyShelf foo_shelf {demo_owner};
    MyBook foo_book_1 {demo_book_title_1, MySubject::other};
    MyPage foo_page_1 {demo_page_title_1, demo_content_1, 1};

    foo_book_1.appendPage(foo_page_1);
    
    if (!foo_shelf.appendNotebook(foo_book_1))
        return 1;

    // 1. Test shelf serialization.

    MySaveWriter saver {demo_save_path};

    if (saver.saveDataFrom(foo_shelf) != SaveStatus::ok)
        return 1;

    // 2. Test shelf loading.

    MySaveLoader loader {demo_save_path};
    MyShelf demo_load {};

    if (!loader.loadDataTo(demo_load))
        return 1;

    if (demo_load.getBookCount() != 1)
        return 1;

    if (demo_load.getOwnerName() != demo_owner)
        return 1;

    auto book_ref = demo_load.getNotebook(demo_book_title_1);

    if (book_ref.getTitle() != demo_book_title_1)
        return 1;

    if (book_ref.getSubject() != MySubject::other)
        return 1;

    auto page_ref = book_ref.getPage(0);

    if (page_ref.getIndex() != 1)
        return 1;

    if (page_ref.getTitle() != demo_page_title_1)
        return 1;

    if (page_ref.getContent() != demo_content_1)
        return 1;
}
