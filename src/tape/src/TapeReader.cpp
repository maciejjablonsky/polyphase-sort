#include "TapeReader.hpp"

TapeReader::TapeReader(const std::string_view in_tape_file_path, const int page_size)
    : reader_(in_tape_file_path, page_size), pages_(reader_.PagesInTapeFile()), pages_read_(0)
{
    ReadNextPage();
}

TapeReader::iterator TapeReader::begin()
{
    return iterator(pages_.front().data(), *this);
}

TapeReader::sentinel TapeReader::end() const
{
    return sentinel{};
}

bool TapeReader::WholeTapeRead() const
{
    return reader_.WasLastPageRead();
}

std::vector<Page> &TapeReader::GetPages()
{
    return pages_;
}

Page &TapeReader::ReadNextPage()
{
    ++pages_read_;
    pages_.at(pages_read_ - 1) = reader_.ReadPage();
    return pages_.at(pages_read_ - 1);
}

bool operator==(const TapeReader::iterator &iter, TapeReader::sentinel s)
{
    return iter.ptr_ == iter.last_;
}

bool operator!=(const TapeReader::iterator &iter, TapeReader::sentinel s)
{
    return !operator==(iter, s);
}

TapeReader::iterator::iterator(std::byte *page_beginning, TapeReader &parent)
    : bytes_offset_(sizeof(PageHeader)),
      ptr_(reinterpret_cast<Record::SerializedRecord *>(page_beginning + bytes_offset_)), tape_reader_(parent)
{
}

TapeReader::iterator &TapeReader::iterator::operator++()
{
    bytes_offset_ += sizeof(Record::SerializedRecord);
    ptr_++;
    if (RecordsReadFromCurrentPage() >= CurrentPage().records_number() && !IsOnLastPage())
    {
        ++current_page_idx_;
        auto & page = tape_reader_.ReadNextPage();
        bytes_offset_ = sizeof(PageHeader);
        ptr_ = reinterpret_cast<TapeReader::tape_item *>(CurrentPage().data() + bytes_offset_);
        if (last_ == nullptr && IsOnLastPage())
        {
            int offset = sizeof(PageHeader) + page.records_number() * sizeof(Record::SerializedRecord);

            last_ = reinterpret_cast<Record::SerializedRecord *>(reinterpret_cast<char *>(page.data()) +
                                                                 offset);
        }
    }

    return *this;
}

TapeReader::tape_item &TapeReader::iterator::operator*()
{
    return *(this->ptr_);
}

int TapeReader::iterator::RecordsReadFromCurrentPage() const
{
    return (bytes_offset_ - sizeof(PageHeader)) / sizeof(Record::SerializedRecord);
}

const Page &TapeReader::iterator::CurrentPage() const
{
    return tape_reader_.pages_.at(current_page_idx_);
}

Page &TapeReader::iterator::CurrentPage()
{
    return const_cast<Page &>(static_cast<const TapeReader::iterator &>(*this).CurrentPage());
}

bool operator==(const TapeReader::iterator &lhs, const TapeReader::iterator &rhs)
{
    return lhs.ptr_ == rhs.ptr_;
}

bool operator!=(const TapeReader::iterator &lhs, const TapeReader::iterator &rhs)
{
    return !(lhs.ptr_ == rhs.ptr_);
}

bool TapeReader::iterator::IsOnLastPage() const
{
    return tape_reader_.WholeTapeRead() && (current_page_idx_ == tape_reader_.pages_.size() - 1);
}
int operator-(const TapeReader::iterator &lhs, const TapeReader::iterator &rhs)
{
    TapeReader::iterator tmp_rhs = rhs;
    int counter = 0;
    while (lhs.ptr_ != tmp_rhs.ptr_) {counter++;}
    return counter;
}