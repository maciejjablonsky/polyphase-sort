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

    if (preparing_page_iterator_ == preparing_page_.end())
    {
        Flush();
        PrepareNewPage();
    }

    *preparing_page_iterator_++ = record;
    ++records_count_;
}

void TapeWriter::Flush()
{
    preparing_page_.WriteStoredRecordsNumber(records_count_);
    writer_.WritePage(std::move(preparing_page_));
    writer_.Flush();
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
