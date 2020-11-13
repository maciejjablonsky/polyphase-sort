#ifndef RECORDS_PAGE_HPP
#define RECORDS_PAGE_HPP

#include "Page.hpp"
#include "Record.hpp"

class RecordsPage
{
  public:
    using stored_item = Record::SerializedRecord;

    class const_iterator
    {
      public:
        const_iterator(const Record::SerializedRecord * ptr, const RecordsPage& parent);
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
    
    RecordsPage(Page&& page);
    const_iterator cbegin();
    const_iterator begin() const;
    const_iterator cend();
    const_iterator end() const;

  private:
    Page page_;
    PageHeader* page_header_;
};

#endif // RECORDS_PAGE_HPP