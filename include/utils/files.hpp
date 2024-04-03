#ifndef FILES_HPP
#define FILES_HPP

/**
 * @brief This contains the serialization & deserialization utilities for notes.
 * Shelf Format:
 *   Header:
 *     Format name: "noteshelf" ASCII
 *     Format version: blob {0x0, 0x1}
 *     Shelf Size: uint8_t
 *     Owner: (string {uint16_t, ASCII})
 *   Book: ...
 *     Index: uint8_t
 *     Subject: uint8_t
 *     Count: uint8_t
 *     Title: string
 *     Page: ...
 *       Index: uint8_t
 *       Title: string
 *       Content: string
 */

#include <string>
#include <fstream>
#include "datatypes/notebook.hpp"
#include "datatypes/bookshelf.hpp"

namespace mynotehub::utils
{
    using MyShelf = mynotehub::datatypes::Bookshelf;
    using MyBook = mynotehub::datatypes::Notebook;
    using MyPage = mynotehub::datatypes::NotePage;

    enum class VersionPart
    {
        major,
        minor
    };

    enum class LoadingState
    {
        idle,
        shelf_save_name,
        shelf_save_vers,
        shelf_size,
        shelf_owner,
        book_begin,
        book_page,
        book_end,
        shelf_end,
        stop_error,
        stop_normal
    };

    enum class SavingStatus
    {
        ok,
        file_io_error,
        invalid_data_error,
        unknown_error
    };

    constexpr uint16_t encodeVersion(uint8_t major, uint8_t minor)
    {
        uint16_t version_code = (major << 8) + minor;

        return version_code;
    }

    constexpr uint8_t decodeXVersion(uint16_t code, VersionPart part)
    {
        constexpr uint8_t mask = 0xff;

        if (part == VersionPart::major)
            return code & (mask << 8);

        return code & mask;
    }

    class LoadMachine
    {
    private:
        std::ifstream reader;
        LoadingState state;

        uint8_t temp_shelf_size;
        std::string temp_owner;

        uint8_t temp_book_index;
        uint8_t temp_book_subject;
        uint8_t temp_book_page_count;
        std::string temp_book_title;

        MyBook temp_book;

        [[nodiscard]] uint8_t readU8();
        [[nodiscard]] uint16_t readU16();
        [[nodiscard]] std::string readText(uint16_t length);

        LoadingState stateSaveName();
        LoadingState stateSaveVersion();
        LoadingState stateShelfSize();
        LoadingState stateShelfOwner();
        LoadingState stateBookBegin();
        LoadingState stateBookPage();
        LoadingState stateBookEnd(MyShelf& shelf);
        LoadingState stateShelfEnd();

    public:
        LoadMachine(const std::string& save_path);

        [[nodiscard]] bool loadDataTo(MyShelf& shelf);

        ~LoadMachine() = default;
    };

    class SaveMachine
    {
    private:
        std::ofstream writer;

        [[nodiscard]] bool writeAsU8(size_t num);
        [[nodiscard]] bool writeAsU16(size_t num);
        [[nodiscard]] bool writeAsString(const std::string& text);

        [[nodiscard]] SavingStatus writeSaveHeading(uint16_t versioning);

        [[nodiscard]] SavingStatus writeBookData(const MyBook& book);

        [[nodiscard]] SavingStatus writePageData(const MyPage& page);

    public:
        SaveMachine(const std::string& save_path);

        [[nodiscard]] SavingStatus saveDataFrom(const MyShelf& shelf);

        ~SaveMachine() = default;
    };
}

#endif