#ifndef TAPE_READER_HPP
#define TAPE_READER_HPP

#include "PageReader.hpp"
#include <string_view>
#include "Record.hpp"
#include <optional>
#include <variant>
#include <queue>
#include "RecordsPage.hpp"

class TapeReader
{
  public:
    using tape_item = Record::SerializedRecord;
    class sentinel
    {
    };
    class const_iterator
    {
      public:
        const_iterator(TapeReader &parent);
        const_iterator &operator++();
        const TapeReader::tape_item &operator*();
        const TapeReader::tape_item &operator->();

        friend bool operator==(const const_iterator &iter, TapeReader::sentinel);
        friend bool operator!=(const const_iterator &iter, TapeReader::sentinel);

      private:
        TapeReader * parent_;
    };

    static_assert(std::is_copy_constructible<const_iterator>());
    static_assert(std::is_copy_assignable<const_iterator>());
    static_assert(std::is_destructible<const_iterator>());
    static_assert(std::is_swappable<const_iterator>());

  public:
    TapeReader(const std::string_view in_tape_file_path, const int page_size);
    TapeReader(std::queue<Page> &&pages);
    const_iterator cbegin();
    const_iterator begin() const;
    sentinel end() const;
    sentinel cend();

    friend bool operator==(const const_iterator &iter, TapeReader::sentinel);
    friend bool operator!=(const const_iterator &iter, TapeReader::sentinel);

    bool Empty() const;

  private:
    void ReadNextPage();
    bool LastPageLoaded() const;

  private:
    std::variant<PageReader, std::queue<Page>> source_;
    RecordsPage current_page_;
    RecordsPage::const_iterator current_page_iterator_;
};

#endif // TAPE_READER_HPP
