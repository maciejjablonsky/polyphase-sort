#include "TapeReader.hpp"

TapeReader::TapeReader(const std::string_view in_tape_file_path, const int page_size)
    : reader_(in_tape_file_path, page_size), bytes_read_(0)
{
    current_page_ = ReadNextPage();
}

bool TapeReader::EndOfTape() const
{
    return RecordsReadFromCurrentPage() == current_page_.records_number() && reader_.WasLastPageRead();
}

Record::SerializedRecord &TapeReader::GetNextRecord()
{
    if (RecordsReadFromCurrentPage() >= current_page_.records_number())
    {
        ReadNextPage();
    }
    Record::SerializedRecord *record = reinterpret_cast<Record::SerializedRecord *>(
        current_page_.data() + sizeof(PageHeader) + 
        (RecordsReadFromCurrentPage() * sizeof(Record::SerializedRecord)));
    bytes_read_ += sizeof(Record::SerializedRecord);
    return *record;
}

int TapeReader::RecordsReadFromCurrentPage() const
{
    return (bytes_read_ - sizeof(PageHeader)) / sizeof(Record::SerializedRecord);
}

Page &TapeReader::ReadNextPage()
{
    current_page_ = reader_.ReadPage();
    bytes_read_ = sizeof(PageHeader);
    return current_page_;
}
