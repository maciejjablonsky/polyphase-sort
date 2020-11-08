#ifndef PAGE_HPP
#define PAGE_HPP

#include <byte.hpp>
#include "TapeHeaders.hpp"

class Page
{
  public:
    Page();
    Page(int page_size);
    Page(byte_vector &&memory);
    Page(std::initializer_list<std::byte> list);
    //Page &operator=(Page &&page) = default;
    int size() const;
    std::byte *data();
    const std::byte *data() const;
    int64_t records_number() const;

  private:
    byte_vector memory_;
    PageHeader * header_;
};

#endif // PAGE_HPP