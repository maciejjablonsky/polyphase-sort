#ifndef TAPE_READER_HPP
#define TAPE_READER_HPP

#include "PageReader.hpp"
#include <string_view>
#include "Record.hpp"
#include <optional>

class TapeReader
{
  public:
    TapeReader(const std::string_view in_tape_file_path, const int page_size);
    bool EndOfTape() const;
    Record::SerializedRecord & GetNextRecord();
  private:
    int RecordsReadFromCurrentPage() const;
    Page & ReadNextPage();
    PageReader reader_;
    int bytes_read_;
    Page current_page_;
};

#endif // TAPE_READER_HPP