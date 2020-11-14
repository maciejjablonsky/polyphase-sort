#ifndef RECORDS_PAGE_HPP
#define RECORDS_PAGE_HPP

#include "Page.hpp"
#include "Record.hpp"
#include <cstdint>

class RecordsPage
{
  public:
    using stored_item = Record::SerializedRecord;

    class iterator
    {
      public:
        iterator(Record::SerializedRecord* ptr, RecordsPage& parent);
        iterator& operator++();
        RecordsPage::stored_item& operator*();
        RecordsPage::stored_item& operator->();

        friend bool operator==(const iterator& lhs, const iterator& rhs);
        friend bool operator!=(const iterator& lhs, const iterator& rhs);

      private:
        RecordsPage* parent_ = nullptr;
        Record::SerializedRecord* ptr_ = nullptr;
        uint64_t records_idx_ = 0;
    };

    class const_iterator
    {
      public:
        const_iterator(const Record::SerializedRecord* ptr, const RecordsPage& parent);
        const_iterator& operator++();
        const RecordsPage::stored_item& operator*();
        const RecordsPage::stored_item& operator->();

        friend bool operator==(const const_iterator& lhs, const const_iterator& rhs);
        friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs);

      private:
        const RecordsPage* parent_ = nullptr;
        const Record::SerializedRecord* ptr_ = nullptr;
        uint64_t records_idx_ = 0;
    };

    static_assert(std::is_copy_constructible<const_iterator>());
    static_assert(std::is_copy_assignable<const_iterator>());
    static_assert(std::is_destructible<const_iterator>());
    static_assert(std::is_swappable<const_iterator>());
    static_assert(std::is_copy_constructible<iterator>());
    static_assert(std::is_copy_assignable<iterator>());
    static_assert(std::is_destructible<iterator>());
    static_assert(std::is_swappable<iterator>());

    RecordsPage(Page&& page);
    const_iterator cbegin();
    const_iterator begin() const;
    const_iterator cend();
    const_iterator end() const;
    iterator begin();
    iterator end();

    void WriteStoredRecordsNumber(uint64_t records);

  private:
    Page page_;
    PageHeader* page_header_;
};

#endif // RECORDS_PAGE_HPP