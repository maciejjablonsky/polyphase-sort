#include "TapeWriter.hpp"
#include <factory_functions.hpp>
#include <fmt/format.h>

TapeWriter::TapeWriter(const std::string_view tape_path, int disk_page_size)
    : writer_(tape_path, disk_page_size), page_size_(disk_page_size), preparing_page_(disk_page_size),
      preparing_page_iterator_(preparing_page_.begin())
{
}

void TapeWriter::Write(const Record::SerializedRecord& record)
{
    if (!is_any_record_saved_)
    {
        is_any_record_saved_ = true;
    }
    else
    {
        ++preparing_page_iterator_;
    }

    if (preparing_page_iterator_ == preparing_page_.end())
    {
        Flush();
        PrepareNewPage();
    }

    *preparing_page_iterator_ = record;
    ++records_count_;
}

const Record::SerializedRecord& TapeWriter::LastWrittenRecord()
{
    if (!is_any_record_saved_)
    {
        return Record::DEFAULT_MAX;
    }
    return *preparing_page_iterator_;
}

void TapeWriter::Flush()
{
    preparing_page_.WriteStoredRecordsNumber(records_count_);
    writer_.WritePage(preparing_page_);
}

TapeWriter::~TapeWriter()
{
    try
    {
        Flush();
    }
    catch (const std::exception& e)
    {
        fmt::print("Failed to print records to tape. Error message: {}", e.what());
        std::exit(-1);
    }
}

void TapeWriter::PrepareNewPage()
{
    preparing_page_ = Page(page_size_);
    preparing_page_iterator_ = preparing_page_.begin();
    records_count_ = 0;
}
