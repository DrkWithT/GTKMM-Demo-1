/**
 * @file notebook.cpp
 * @author DrkWithT
 * @brief Implements notebook data classes.
 * @date 2024-03-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <utility>
#include "datatypes/notebook.hpp"

namespace mynotehub::datatypes
{
    constexpr const char* placeholder_title = "Unknown";

    NotePage::NotePage(const std::string& title_str, const std::string& content_str, int index_arg)
    : title {title_str}, content {content_str}, index {index_arg} {}

    const std::string& NotePage::getTitle() const
    {
        return title;
    }

    const std::string& NotePage::getContent() const
    {
        return content;
    }

    int NotePage::getIndex() const
    {
        return index;
    }

    Notebook::Notebook()
    : pages {}, title {placeholder_title}, subject {SubjectType::unknown} {}

    Notebook::Notebook(const std::string& title_str)
    : pages {}, title {title_str}, subject {SubjectType::unknown} {}

    Notebook::Notebook(const std::string& title_str, SubjectType subject_code)
    : pages {}, title {title_str}, subject {subject_code} {}

    const std::vector<NotePage>& Notebook::getAllPages() const
    {
        return pages;
    }

    size_t Notebook::getPageCount() const
    {
        return pages.size();
    }

    const std::string& Notebook::getTitle() const
    {
        return title;
    }

    SubjectType Notebook::getSubject() const
    {
        return subject;
    }

    const NotePage& Notebook::getPage(size_t index) const
    {
        return pages.at(index);
    }

    void Notebook::appendPage(const NotePage& page)
    {
        pages.push_back(page);
    }

    [[nodiscard]] bool Notebook::removePage(int index)
    {
        auto page_location = pages.begin();

        while (page_location != pages.end())
        {
            if (page_location->getIndex() == index)
            {
                pages.erase(page_location);
                return true;
            }
        }

        return false;
    }

    void Notebook::clearPages()
    {
        pages.clear();
    }
}
