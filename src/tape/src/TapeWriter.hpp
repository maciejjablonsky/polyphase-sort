#ifndef TAPEWRITER_HPP
#define TAPEWRITER_HPP
#include "Record.hpp"
#include "PageWriter.hpp"
#include <vector>
#include <string_view>
#include <memory>
class TapeWriter
{
  public:
    TapeWriter(const std::string_view tape_path, int disk_page_size);
    void Write(const Record::SerializedRecord & record);
    void Flush();
    ~TapeWriter();

  private:
    PageWriter writer_;
    int page_size_;
    Page preparing_page_;
    int records_count_;
};

#endif // TAPEWRITER_HPP
