#include "TapeWriter.hpp"
#include <factory_functions.hpp>

TapeWriter::TapeWriter(const std::string_view tape_path, int disk_page_size)
    : tape_stream_(tape_path.data(), std::ios::out | std::ios::binary), disk_page_size_(disk_page_size)
{
}

void TapeWriter::Write(std::vector<Record::SerializedRecord> &&serialized_records)
{
    records_vector_buffer_.push_back(std::move(serialized_records));
}

void TapeWriter::Flush()
{
    for(const auto & records : records_vector_buffer_)
    {
        tape_stream_.write(reinterpret_cast<const char *>(records.data()),
                           funcs::vectorsizeof(records));
    }
    records_vector_buffer_.clear();
}

TapeWriter::~TapeWriter()
{
    if (tape_stream_.good())
    {
        Flush();
    }
}
