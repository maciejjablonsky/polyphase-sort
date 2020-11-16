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
      public:
        iterator(std::byte* ptr, const uint64_t size);
        friend bool operator==(const iterator& lhs, const iterator& rhs);
        friend bool operator!=(const iterator& lhs, const iterator& rhs);

        std::byte& operator*();
        iterator& operator++();
        iterator operator++(int);
        iterator operator+(uint64_t);
        void operator+=(uint64_t);
        iterator operator-(uint64_t);

      private:
        std::byte* ptr_;
        uint64_t page_size_;
    };
    class const_iterator
    {
      public:
        const_iterator(const std::byte* ptr, const uint64_t size);
        friend bool operator==(const const_iterator& lhs, const const_iterator& rhs);
        friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs);

        const std::byte& operator*();
        const_iterator& operator++();
        const_iterator operator++(int);
        const_iterator operator+(int);
        void operator+=(uint64_t);
        const_iterator operator-(uint64_t);

      private:
        const std::byte* ptr_;
        uint64_t page_size_;
    };

    Page();
    Page(uint64_t page_size);
    Page(std::initializer_list<std::byte> list);
    int size() const;
    std::byte* data();
    const std::byte* data() const;

    iterator begin();
    iterator end();

    const_iterator cbegin();
    const_iterator begin() const;
    const_iterator cend();
    const_iterator end() const;

  private:
    byte_vector memory_;
};

static_assert(std::is_move_constructible<Page>());
static_assert(std::is_move_assignable<Page>());
static_assert(std::is_copy_assignable<Page>());
static_assert(std::is_copy_assignable<Page>());
static_assert(std::is_nothrow_move_assignable<Page>());
static_assert(std::is_nothrow_move_constructible<Page>());
#endif // PAGE_HPP