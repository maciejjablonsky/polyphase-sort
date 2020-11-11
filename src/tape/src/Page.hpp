#ifndef PAGE_HPP
#define PAGE_HPP

#include <byte.hpp>
#include "TapeHeaders.hpp"

class Page
{
  public:
    class iterator
    {
      public:
        iterator(std::byte* ptr, Page &parent);
        friend bool operator==(iterator lhs, iterator rhs);
        friend bool operator!=(iterator lhs, iterator rhs);
        iterator & operator++();
      private:
        std::byte *ptr_;
        Page &page_;

    };
    Page();
    Page(int page_size);
    Page(byte_vector &&memory);
    Page(std::initializer_list<std::byte> list);
    int size() const;
    std::byte *data();
    const std::byte *data() const;
    int64_t records_number() const;
    iterator begin();
    iterator end();

  private:
    byte_vector memory_;
    PageHeader * header_;
};

#endif // PAGE_HPP