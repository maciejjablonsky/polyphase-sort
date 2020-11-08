#include "TapeReader.hpp"

TapeReader::TapeReader(const std::string_view in_tape_file_path, const int page_size)
    : reader_(in_tape_file_path, page_size), bytes_read_(0), pages_(reader_.PagesInTapeFile()),
      current_page_idx_(-1)
{
    ReadNextPage();
}

bool TapeReader::EndOfTape() const
{
    return RecordsReadFromCurrentPage() == CurrentPage().records_number() && reader_.WasLastPageRead();
}

Record::SerializedRecord &TapeReader::GetNextRecord()
{
    if (RecordsReadFromCurrentPage() >= CurrentPage().records_number())
    {
        ReadNextPage();
    }
    Record::SerializedRecord *record = reinterpret_cast<Record::SerializedRecord *>(
        CurrentPage().data() + sizeof(PageHeader) + 
        (RecordsReadFromCurrentPage() * sizeof(Record::SerializedRecord)));
    bytes_read_ += sizeof(Record::SerializedRecord);
    return *record;
}

int TapeReader::RecordsReadFromCurrentPage() const
{
    return (bytes_read_ - sizeof(PageHeader)) / sizeof(Record::SerializedRecord);
}

Page &TapeReader::CurrentPage()
{
    return const_cast<Page&>(static_cast<const TapeReader &>(*this).CurrentPage());
}

const Page &TapeReader::CurrentPage() const
{
    return pages_.at(current_page_idx_);
}

Page &TapeReader::ReadNextPage()
{
    ++current_page_idx_;
    pages_.at(current_page_idx_) =  reader_.ReadPage();
    bytes_read_ = sizeof(PageHeader);
    return pages_.at(current_page_idx_);
}
