#ifndef PAGE_HPP
#define PAGE_HPP

#include <byte.hpp>
#include "TapeHeaders.hpp"

class Page
{
  public:
    class const_iterator;
    class iterator
    {
      public:
        iterator(std::byte *ptr, std::byte *ending);
        friend bool operator==(const iterator &lhs, const iterator &rhs);
        friend bool operator!=(const iterator &lhs, const iterator &rhs);
        std::byte &operator*();
        iterator &operator++();
        iterator operator++(int);
        iterator operator+(int);

        friend bool operator==(const Page::iterator &lhs, const Page::const_iterator &rhs);
        friend bool operator!=(const Page::iterator &lhs, const Page::const_iterator &rhs);

      private:
        std::byte *ptr_;
        std::byte *end_;
    };
    class const_iterator
    {
      public:
        const_iterator(const std::byte *ptr, const std::byte *ending);
        friend bool operator==(const const_iterator &lhs, const const_iterator &rhs);
        friend bool operator!=(const const_iterator &lhs, const const_iterator &rhs);

        friend bool operator==(const Page::iterator &lhs, const Page::const_iterator &rhs);
        friend bool operator!=(const Page::iterator &lhs, const Page::const_iterator &rhs);

        const std::byte &operator*();
        const_iterator &operator++();
        const_iterator operator++(int);
        const_iterator operator+(int);

      private:
        const std::byte *ptr_;
        const std::byte *end_;
    };

    Page();
    Page(int page_size);
    Page(byte_vector &&memory);
    Page(std::initializer_list<std::byte> list);
    int size() const;
    std::byte *data();
    const std::byte *data() const;
    int64_t records_number() const;
    void SetRecordsNumber(int stored_records);
    void Append(const std::byte *bytes, const int size);
    void Append(const byte_vector &bytes);
    bool IsFull() const;

    iterator begin();
    iterator end();
    const_iterator cbegin() const;
    const_iterator cend() const;

  private:
    byte_vector memory_;
    PageHeader *header_;
    iterator occupied_memory_end_;
};

bool operator==(const Page::iterator &lhs, const Page::const_iterator &rhs);
bool operator!=(const Page::iterator &lhs, const Page::const_iterator &rhs);

#endif // PAGE_HPP