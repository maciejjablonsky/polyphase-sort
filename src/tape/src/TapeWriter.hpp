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
    std::unique_ptr<Page> buffered_page_;
    int page_fill_index_;
};

#endif // TAPEWRITER_HPP
