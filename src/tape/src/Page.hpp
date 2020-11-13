#ifndef PAGE_HPP
#define PAGE_HPP

#include <byte.hpp>
#include "TapeHeaders.hpp"

class Page
{
  public:
    static constexpr uint64_t DEFAULT_PAGE_SIZE = 4096;
    class iterator
    {
    };
    class const_iterator
    {
      public:
        const_iterator(const std::byte *ptr, const uint64_t size);
        friend bool operator==(const const_iterator &lhs, const const_iterator &rhs);
        friend bool operator!=(const const_iterator &lhs, const const_iterator &rhs);

        const std::byte &operator*();
        const_iterator &operator++();
        const_iterator operator++(int);
        const_iterator operator+(int);
        void operator+=(uint64_t);
        const_iterator operator-(uint64_t);

      private:
        const std::byte *ptr_;
        uint64_t page_size_; 
    };

    Page();
    Page(uint64_t page_size);
    Page(byte_vector &&memory);
    Page(std::initializer_list<std::byte> list);
    int size() const;
    std::byte *data();
    const std::byte *data() const;

    const_iterator cbegin();
    const_iterator begin() const;
    const_iterator cend();
    const_iterator end() const;

  private:
    byte_vector memory_;
};
#endif // PAGE_HPP