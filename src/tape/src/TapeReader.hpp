#ifndef TAPE_READER_HPP
#define TAPE_READER_HPP

#include "PageReader.hpp"
#include <string_view>
#include "Record.hpp"
#include <optional>

class TapeReader
{
  public:
    using tape_iterator = Record::SerializedRecord *;
    using tape_const_iterator = const Record::SerializedRecord *;
    TapeReader(const std::string_view in_tape_file_path, const int page_size);
    bool EndOfTape() const;
    Record::SerializedRecord &GetNextRecord();
  private:
    int RecordsReadFromCurrentPage() const;
    Page &CurrentPage();
    const Page &CurrentPage() const;
    Page &ReadNextPage();
    PageReader reader_;
    int bytes_read_;
    std::vector<Page> pages_;
    int current_page_idx_;
};

#endif // TAPE_READER_HPP