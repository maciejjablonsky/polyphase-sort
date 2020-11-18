#ifndef TAPEWRITER_HPP
#define TAPEWRITER_HPP
#include "Record.hpp"
#include "PageWriter.hpp"
#include <vector>
#include <string_view>
#include <memory>
#include "RecordsPage.hpp"
#include <type_traits>

class TapeWriter
{
  public:
    TapeWriter(const std::string_view tape_path, int disk_page_size);
    TapeWriter(TapeWriter&& writer) = default;
    TapeWriter& operator=(TapeWriter&& writer) = default;
    void Write(const Record::SerializedRecord& record);
    const Record::SerializedRecord & LastWrittenRecord();
    void Flush();
    ~TapeWriter();

  private:
    void PrepareNewPage();
    PageWriter writer_;
    int page_size_;
    RecordsPage preparing_page_;
    RecordsPage::iterator preparing_page_iterator_;
    uint64_t records_count_ = 0;
    bool is_any_record_saved_ = false;
};

static_assert(std::is_move_assignable<TapeWriter>());
static_assert(std::is_move_constructible<TapeWriter>());

#endif // TAPEWRITER_HPP
