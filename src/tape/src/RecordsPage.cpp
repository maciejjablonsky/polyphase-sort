#include "RecordsPage.hpp"
#include <stdexcept>

RecordsPage::const_iterator::const_iterator(const Record::SerializedRecord* ptr, const RecordsPage& parent)
    : parent_(&parent), ptr_(ptr)
{
}

RecordsPage::const_iterator& RecordsPage::const_iterator::operator++()
{
    if (records_idx_ >= parent_->page_header_->inserted_records_number)
    {
        throw std::out_of_range("No more records on the page.");
    }
    ++records_idx_;
    ++ptr_;
    return *this;
}

const RecordsPage::stored_item& RecordsPage::const_iterator::operator*()
{
    return *ptr_;
}

const RecordsPage::stored_item& RecordsPage::const_iterator::operator->()
{
    return *ptr_;
}

bool operator==(const RecordsPage::const_iterator& lhs, const RecordsPage::const_iterator& rhs)
{
    return lhs.ptr_ == rhs.ptr_;
}

bool operator!=(const RecordsPage::const_iterator& lhs, const RecordsPage::const_iterator& rhs)
{
    return !(lhs == rhs);
}

RecordsPage::RecordsPage(Page&& page)
    : page_(std::move(page)), page_header_(reinterpret_cast<PageHeader*>(page_.data()))
{
}

RecordsPage::const_iterator RecordsPage::cbegin()
{
    return {reinterpret_cast<const Record::SerializedRecord*>(page_.data() + sizeof(PageHeader)), *this};
}

RecordsPage::const_iterator RecordsPage::begin() const
{
    return const_cast<RecordsPage&>(*this).cbegin();
}

RecordsPage::const_iterator RecordsPage::cend()
{
    return {reinterpret_cast<const Record::SerializedRecord*>(page_.data() + sizeof(PageHeader) +
                                                              page_header_->inserted_records_number *
                                                                  sizeof(Record::SerializedRecord)),
            *this};
}

RecordsPage::const_iterator RecordsPage::end() const
{
    return const_cast<RecordsPage&>(*this).cend();
}

RecordsPage::iterator RecordsPage::begin()
{
    return {reinterpret_cast<Record::SerializedRecord*>(page_.data() + sizeof(PageHeader)), *this};
}

RecordsPage::iterator RecordsPage::end()
{
    return {reinterpret_cast<Record::SerializedRecord*>(
                page_.data() + sizeof(PageHeader) +
                ((page_.size() - sizeof(PageHeader)) / sizeof(Record::SerializedRecord)) *
                    sizeof(Record::SerializedRecord)),
            *this};
}

RecordsPage::operator Page()
{
    return page_;
}

RecordsPage& RecordsPage::operator=(Page&& page)
{
    page_ = std::move(page);
    page_header_ = reinterpret_cast<PageHeader*>(page_.data());
    return *this;
}

void RecordsPage::WriteStoredRecordsNumber(uint64_t records)
{
    page_header_->inserted_records_number = records;
}

RecordsPage::iterator::iterator(Record::SerializedRecord* ptr, RecordsPage& parent)
    : parent_(&parent), ptr_(ptr)
{
}

RecordsPage::iterator& RecordsPage::iterator::operator++()
{
    ++ptr_;
    return* this;
}

RecordsPage::stored_item& RecordsPage::iterator::operator*()
{
    return *ptr_;
}

RecordsPage::stored_item& RecordsPage::iterator::operator->()
{
    return **this;
}

bool operator==(const RecordsPage::iterator & lhs, const RecordsPage::iterator & rhs)
{
    return lhs.ptr_ == rhs.ptr_;
}

bool operator!=(const RecordsPage::iterator& lhs, const RecordsPage::iterator& rhs)
{
    return !(lhs == rhs);
}
