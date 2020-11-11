#include "TapeWriter.hpp"
#include <factory_functions.hpp>
#include <fmt/format.h>

TapeWriter::TapeWriter(const std::string_view tape_path, int disk_page_size)
    : writer_(tape_path, disk_page_size), page_size_(disk_page_size), records_count_(0), preparing_page_(disk_page_size)
{
}

void TapeWriter::Write(const Record::SerializedRecord &record)
{

    preparing_page_.Append(reinterpret_cast<const std::byte *>(&record), sizeof(record));
    ++records_count_;

    if (preparing_page_.IsFull())
    {
        preparing_page_.SetRecordsNumber(records_count_);
        writer_.WritePage(std::move(preparing_page_));
        preparing_page_ = Page(page_size_);
        records_count_ = 0;
    }
}

void TapeWriter::Flush()
{
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
