#ifndef NOTEBOOK_HPP
#define NOTEBOOK_HPP

#include <string>
#include <vector>

namespace mynotehub::datatypes
{
    enum class SubjectType
    {
        school,
        programming,
        fun,
        other,
        unknown
    };

    class NotePage
    {
    private:
        std::string title;
        std::string content;
        int index;

    public:
        NotePage() = delete;
        NotePage(const std::string& title_str, const std::string& content_str, int index_arg);

        const std::string& getTitle() const;
        const std::string& getContent() const;
        int getIndex() const;
    };

    class Notebook
    {
    private:
        std::vector<NotePage> pages;
        std::string title;
        SubjectType subject;

    public:
        Notebook();
        Notebook(const std::string& title_str);
        Notebook(const std::string& title_str, SubjectType subject_code);

        const std::vector<NotePage>& getAllPages() const;
        size_t getPageCount() const;
        const std::string& getTitle() const;
        SubjectType getSubject() const;

        const NotePage& getPage(size_t index) const;
        void appendPage(const NotePage& page);
        [[nodiscard]] bool removePage(int index);
        void clearPages();
    };
}

#endif