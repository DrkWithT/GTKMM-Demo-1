/**
 * @file files.cpp
 * @author DrkWithT
 * @brief Implements save file utilities.
 * @version 0.1
 * @date 2024-04-01
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <algorithm>
#include <memory>
#include <string_view>
#include "utils/files.hpp"

namespace mynotehub::utils
{
    using MySubject = mynotehub::datatypes::SubjectType;
    using MyNotepage = mynotehub::datatypes::NotePage;
    using MyNotebook = mynotehub::datatypes::Notebook;
    using MyBookshelf = mynotehub::datatypes::Bookshelf;
    using MyLMState = mynotehub::utils::LoadingState;

    constexpr const char* format_name = "noteshelf";
    constexpr size_t format_name_len = 10;
    constexpr uint16_t format_version = 0x0001;

    constexpr size_t max_text_size = 0xffff;
    constexpr size_t max_book_save_count = 0xff;
    constexpr size_t max_pages_per_book = 0xff;

    /* LoadMachine */

    [[nodiscard]] uint8_t LoadMachine::readU8()
    {
        uint8_t sb = 0;

        reader >> sb;

        return sb;
    }

    [[nodiscard]] uint16_t LoadMachine::readU16()
    {
        uint8_t upper_octet = 0;
        uint8_t lower_octet = 0;

        reader >> upper_octet;
        reader >> lower_octet;

        uint16_t result = (upper_octet << 8) + lower_octet;

        return result;
    }

    [[nodiscard]] std::string LoadMachine::readText(uint16_t length)
    {
        auto temp_buffer = std::make_unique<char[]>(length + 1);
        std::fill(temp_buffer.get(), temp_buffer.get() + length, '\0');

        reader.readsome(temp_buffer.get(), length);

        return std::string {temp_buffer.get()};
    }

    LoadingState LoadMachine::stateSaveName()
    {
        char name_buffer[format_name_len + 1];
        std::fill(name_buffer, name_buffer + format_name_len, '\0');

        reader.readsome(name_buffer, format_name_len);

        std::string_view name_sv {name_buffer};

        return (name_sv == format_name)
            ? MyLMState::shelf_save_vers
            : MyLMState::stop_error;
    }

    LoadingState LoadMachine::stateSaveVersion()
    {
        uint16_t version_code = readU16();

        return (version_code == format_version)
            ? MyLMState::shelf_size
            : MyLMState::stop_error;
    }

    LoadingState LoadMachine::stateShelfSize()
    {
        temp_shelf_size = readU8();

        return MyLMState::shelf_owner;
    }

    LoadingState LoadMachine::stateShelfOwner()
    {
        uint16_t owner_name_len = readU16();

        temp_owner = readText(owner_name_len);

        return MyLMState::book_begin;
    }

    LoadingState LoadMachine::stateBookBegin()
    {
        uint8_t book_index = readU8();
        uint8_t subject_code = readU8();
        uint8_t page_count = readU8();
        uint16_t book_title_len = readU16();
        temp_book_title = readText(book_title_len);

        temp_book_index = book_index;
        temp_book_subject = subject_code;
        temp_book_page_count = page_count;

        temp_book = MyBook(temp_book_title);

        return MyLMState::book_page;
    }

    LoadingState LoadMachine::stateBookPage()
    {
        if (temp_book_page_count < 1)
            return MyLMState::book_end;

        uint8_t page_index = readU8();
        uint16_t title_len = readU16();
        std::string title_str = readText(title_len);

        uint16_t content_len = readU16();
        std::string content_str = readText(content_len);

        auto page = MyPage(title_str, content_str, temp_book.getPageCount() + 1);
        temp_book.appendPage(page);
        temp_book_page_count--;

        return MyLMState::book_page;
    }

    LoadingState LoadMachine::stateBookEnd(MyShelf& shelf)
    {
        if (temp_shelf_size < 1)
            return MyLMState::shelf_end;

        shelf.appendNotebook(temp_book);
        temp_shelf_size--;

        return MyLMState::book_begin;
    }

    LoadingState LoadMachine::stateShelfEnd()
    {
        if (!reader.eof() || reader.bad())
            return MyLMState::stop_error;

        return MyLMState::stop_normal;
    }

    LoadMachine::LoadMachine(const std::string& save_path)
    : reader {save_path, std::ios::binary}, state {MyLMState::idle}, temp_shelf_size {0}, temp_owner {}, temp_book_index {0}, temp_book_subject {4}, temp_book_page_count {0}, temp_book_title {} {}

    [[nodiscard]] bool LoadMachine::loadDataTo(MyShelf& shelf)
    {
        while (state != MyLMState::stop_error && state != MyLMState::stop_normal)
        {
            switch (state)
            {
            case MyLMState::idle:
                state = MyLMState::shelf_save_name;
                break;
            case MyLMState::shelf_save_name:
                state = stateSaveName();
                break;
            case MyLMState::shelf_save_vers:
                state = stateSaveVersion();
                break;
            case MyLMState::shelf_size:
                state = stateShelfSize();
                break;
            case MyLMState::shelf_owner:
                state = stateShelfOwner();
                break;
            case MyLMState::book_begin:
                state = stateBookBegin();
                break;
            case MyLMState::book_page:
                state = stateBookPage();
                break;
            case MyLMState::book_end:
                state = stateBookEnd(shelf);
                break;
            case MyLMState::shelf_end:
                state = stateShelfEnd();
                break;
            case MyLMState::stop_normal:
            case MyLMState::stop_error:
            default:
                break;
            }
        }

        return state == MyLMState::stop_normal;
    }

    /* SaveMachine */

    [[nodiscard]] bool SaveMachine::writeAsU8(size_t num)
    {
        uint8_t octet = num & 0xff;

        writer << octet;

        return writer.good();
    }

    [[nodiscard]] bool SaveMachine::writeAsU16(size_t num)
    {
        uint8_t upper_octet = num & 0xff00;
        uint8_t lower_octet = num & 0x00ff;

        writer << upper_octet << lower_octet;

        return writer.good();
    }

    [[nodiscard]] bool SaveMachine::writeAsString(const std::string& text)
    {
        size_t text_size = text.size();

        if (text_size > max_text_size)
            return false;

        if (!writeAsU16(text_size))
            return false;

        for (const auto& c : text)
        {
            writer << c;

            if (writer.bad())
                return false;
        }

        return true;
    }

    [[nodiscard]] SavingStatus SaveMachine::writeSaveHeading(uint16_t versioning)
    {
        if (writer.write(format_name, format_name_len).bad())
            return SavingStatus::file_io_error;

        if (!writeAsU16(versioning))
            return SavingStatus::file_io_error;

        return SavingStatus::ok;
    }

    [[nodiscard]] SavingStatus SaveMachine::writeBookData(const MyBook& book)
    {
        auto book_cursor = book.getAllPages().begin();
        auto book_end = book.getAllPages().end();
        SavingStatus write_status = SavingStatus::ok;

        for (; book_cursor != book_end && write_status == SavingStatus::ok; book_cursor++)
            write_status = writePageData(*book_cursor);

        return write_status;
    }

    [[nodiscard]] SavingStatus SaveMachine::writePageData(const MyPage& page)
    {
        // index, title, content
        size_t page_index = page.getIndex();

        if (page_index > max_pages_per_book)
            return SavingStatus::invalid_data_error;

        if (!writeAsU8(page_index))
            return SavingStatus::file_io_error;

        if (!writeAsString(page.getTitle()))
            return SavingStatus::file_io_error;

        if (!writeAsString(page.getContent()))
            return SavingStatus::file_io_error;

        return SavingStatus::ok;
    }

    SaveMachine::SaveMachine(const std::string& save_path)
    : writer {save_path, std::ios::binary} {}

    [[nodiscard]] SavingStatus SaveMachine::saveDataFrom(const MyShelf& shelf)
    {
        // 1. Write save-file metadata.

        size_t shelf_book_count = shelf.getBookCount();

        if (shelf_book_count > max_book_save_count)
            return SavingStatus::invalid_data_error;

        if (writeSaveHeading(format_version) != SavingStatus::ok)
            return SavingStatus::file_io_error;

        if (!writeAsU8(shelf_book_count))
            return SavingStatus::file_io_error;

        if (!writeAsString(shelf.getOwnerName()))
            return SavingStatus::file_io_error;

        // 2. Serialize each book and its pages.

        auto shelf_cursor = shelf.getAllBooks().begin();
        auto shelf_end = shelf.getAllBooks().end();
        SavingStatus status = SavingStatus::ok;

        for (; shelf_cursor != shelf_end && status == SavingStatus::ok; shelf_cursor++)
            status = writeBookData(*shelf_cursor);

        return SavingStatus::ok;
    }
}
