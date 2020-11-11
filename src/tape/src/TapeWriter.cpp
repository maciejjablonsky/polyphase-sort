#include "TapeWriter.hpp"
#include <factory_functions.hpp>

TapeWriter::TapeWriter(const std::string_view tape_path, int disk_page_size)
    : writer_(tape_path, disk_page_size), page_size_(disk_page_size), page_fill_index_(0)
{
}

void TapeWriter::Write(const Record::SerializedRecord &record)
{
}

void TapeWriter::Flush()
{
}

TapeWriter::~TapeWriter()
{
}
