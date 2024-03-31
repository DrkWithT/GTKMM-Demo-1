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

    /// TODO: Implement notebook class...

    Notebook::Notebook()
    : title {placeholder_title}, pages {}, subject {SubjectType::unknown} {}

    Notebook::Notebook(const std::string& title_str)
    : title {title_str}, pages {}, subject {SubjectType::unknown} {}

    Notebook::Notebook(const std::string& title_str, SubjectType subject_code)
    : title {title_str}, pages {}, subject {subject_code} {}

    size_t Notebook::getPageCount() const
    {
        return pages.size();
    }

    const std::string& Notebook::getTitle() const
    {
        return title;
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
        if (index >= getPageCount())
        {
            return false;
        }

        pages.erase(pages.begin() + index);
        return true;
    }

    void Notebook::clearPages()
    {
        pages.clear();
    }
}
