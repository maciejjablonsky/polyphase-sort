#ifndef TAPE_READER_HPP
#define TAPE_READER_HPP

#include "PageReader.hpp"
#include <string_view>
#include "Record.hpp"
#include <optional>
#include <ranges>

class TapeReader
{
  public:
    using tape_item = Record::SerializedRecord;
    class sentinel
    {
    };
    class iterator
    {
      public:
        iterator(std::byte *page_beginning, TapeReader &parent);
        iterator &operator++();
        //iterator &operator++(int);
        // iterator &operator--();
        // iterator &operator--(int);
        tape_item &operator*();
        friend bool operator==(const iterator &lhs, const iterator &rhs);
        friend bool operator!=(const iterator &lhs, const iterator &rhs);
        friend bool operator==(const iterator &iter, sentinel);
        friend bool operator!=(const iterator &iter, sentinel);

      private:
        int RecordsReadFromCurrentPage() const;
        const Page &CurrentPage() const;
        Page &CurrentPage();

      private:
        TapeReader &tape_reader_;
        int bytes_offset_ = 0;
        int current_page_idx_ = 0;
        tape_item *ptr_ = nullptr; // end iterator has initial value as nullptr
        Record::SerializedRecord *last_ = nullptr;
    };

  public:
    TapeReader(const std::string_view in_tape_file_path, const int page_size);
    iterator begin();
    sentinel end() const;
    friend class TapeReader::iterator;
    bool WholeTapeRead() const;
    std::vector<Page> &GetPages();

  private:
    Page &ReadNextPage();

  private:
    PageReader reader_;
    int bytes_offset_;
    std::vector<Page> pages_;
    int pages_read_;
};


#endif // TAPE_READER_HPP
