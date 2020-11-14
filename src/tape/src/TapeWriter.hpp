#ifndef TAPEWRITER_HPP
#define TAPEWRITER_HPP
#include "Record.hpp"
#include "PageWriter.hpp"
#include <vector>
#include <string_view>
#include <memory>
#include "RecordsPage.hpp"

class TapeWriter
{
  public:
    TapeWriter(const std::string_view tape_path, int disk_page_size);
    void Write(const Record::SerializedRecord & record);
    void Flush();
    ~TapeWriter();

  private:
    void PrepareNewPage();
    PageWriter writer_;
    int page_size_;
    RecordsPage preparing_page_;
    RecordsPage::iterator preparing_page_iterator_;
    uint64_t records_count_ = 0;
};

#endif // TAPEWRITER_HPP
