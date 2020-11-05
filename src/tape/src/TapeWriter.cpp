#include "TapeWriter.hpp"
#include <factory_functions.hpp>

TapeWriter::TapeWriter(const std::string_view tape_path, int disk_page_size)
    : tape_stream_(tape_path.data(), std::ios::out | std::ios::binary), disk_page_size_(disk_page_size)
{
}

void TapeWriter::Write(std::vector<Record::SerializedRecord> &&serialized_records)
{
    vectors_of_serialized_records_.push_back(std::move(serialized_records));
}


TapeWriter::~TapeWriter()
{
    if (tape_stream_.good())
    {
        for (const auto &records_vector : vectors_of_serialized_records_)
        {
            tape_stream_.write(reinterpret_cast<const char *>(records_vector.data()), factory_funcs::vectorsizeof(records_vector));
        }
    }
}

