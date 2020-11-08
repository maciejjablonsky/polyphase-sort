#ifndef PAGE_HPP
#define PAGE_HPP

#include <byte.hpp>
#include "TapeHeaders.hpp"

class Page
{
  public:
    Page(int page_size);
    Page(byte_vector &&memory);
    Page(std::initializer_list<std::byte> list);
    int size() const;
    std::byte *data();
    const std::byte *data() const;
    int records_number() const;

  private:
    PageHeader * header_;
    byte_vector memory_;
};

#endif