#ifndef TAPEWRITER_HPP
#define TAPEWRITER_HPP
#include "Record.hpp"
#include <vector>
#include <string_view>
#include <fstream>
#include <queue>

class TapeWriter
{
    using vector_of_serialized_records = std::vector<Record::SerializedRecord>;
  public:
    TapeWriter(const std::string_view tape_path, int disk_page_size);
    void Write(std::vector<Record::SerializedRecord> &&serialized_records);
    void Flush();
    ~TapeWriter();

  private:
    const int disk_page_size_;
    std::fstream tape_stream_;
    std::vector<vector_of_serialized_records> records_vector_buffer_;
};

#endif // TAPEWRITER_HPP
