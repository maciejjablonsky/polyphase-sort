#include "Page.hpp"
#include <algorithm>
#include <stdexcept>

Page::Page()
    : memory_(sizeof(PageHeader)), header_(reinterpret_cast<PageHeader *>(memory_.data())),
      occupied_memory_end_(memory_.data() + memory_.size(), memory_.data() + memory_.capacity())
{
}

Page::Page(int page_size)
    : memory_(std::max(page_size, static_cast<int>(sizeof(PageHeader)))),
      header_(reinterpret_cast<PageHeader *>(memory_.data())),
      occupied_memory_end_(memory_.data() + sizeof(PageHeader), memory_.data() + memory_.capacity())
{
}

Page::Page(byte_vector &&memory)
    : memory_(std::move(memory)), header_(reinterpret_cast<PageHeader *>(memory_.data())),
      occupied_memory_end_(memory_.data() + memory_.size(), memory_.data() + memory_.capacity())
{
}
Page::Page(std::initializer_list<std::byte> list)
    : memory_(list), header_(reinterpret_cast<PageHeader *>(memory_.data())),
      occupied_memory_end_(memory_.data() + memory_.size(), memory_.data() + memory_.capacity())
{
}

int Page::size() const
{
    return memory_.size();
}

std::byte *Page::data()
{
    return memory_.data();
}

const std::byte *Page::data() const
{
    return memory_.data();
}

int64_t Page::records_number() const
{
    return header_->inserted_records_number;
}

Page::iterator Page::begin()
{
    return {memory_.data(), memory_.data() + memory_.capacity()};
}

Page::iterator Page::end()
{
    return {memory_.data() + memory_.capacity(), memory_.data() + memory_.capacity()};
}

Page::const_iterator Page::cbegin() const
{
    return {memory_.data(), memory_.data() + memory_.capacity()};
}

Page::const_iterator Page::cend() const
{
    return {memory_.data() + memory_.capacity(), memory_.data() + memory_.capacity()};
}

void Page::SetRecordsNumber(int stored_records)
{
    header_->inserted_records_number = static_cast<decltype(header_->inserted_records_number)>(stored_records);
}

void Page::Append(const std::byte *bytes, const int size)
{
    for (int i = 0; i < size; ++i)
    {
        *occupied_memory_end_++ = bytes[i];
    }
}

void Page::Append(const byte_vector &bytes)
{
    Append(bytes.data(), bytes.size());
}

bool Page::IsFull() const
{
    return occupied_memory_end_ == cend();
}

bool operator==(const Page::const_iterator &lhs, const Page::const_iterator &rhs)
{
    return lhs.ptr_ == rhs.ptr_;
}

bool operator!=(const Page::const_iterator &lhs, const Page::const_iterator &rhs)
{
    return !(lhs.ptr_ == rhs.ptr_);
}

bool operator==(const Page::iterator &lhs, const Page::const_iterator &rhs)
{
    return lhs.ptr_ == rhs.ptr_;
}

bool operator!=(const Page::iterator &lhs, const Page::const_iterator &rhs)
{
    return lhs.ptr_ != rhs.ptr_;
}

Page::iterator::iterator(std::byte *ptr, std::byte *ending) : ptr_(ptr), end_(ending)
{
}

Page::iterator &Page::iterator::operator++()
{
    if (ptr_ == end_)
    {
        throw std::runtime_error("Cannot read past the page's memory.");
    }
    ++ptr_;
    return *this;
}

Page::iterator Page::iterator::operator++(int)
{
    auto tmp = *this;
    ++(*this);
    return tmp;
}

Page::iterator Page::iterator::operator+(int offset)
{
    if (ptr_ + offset >= end_)
    {
        throw std::out_of_range("Cannot read past the page's memory.");
    }

    auto tmp = *this;
    for (int i = 0; i < offset; ++i, ++tmp)
        ;
    return tmp;
}

std::byte &Page::iterator::operator*()
{
    return *ptr_;
}

Page::const_iterator::const_iterator(const std::byte *ptr, const std::byte *ending) : ptr_(ptr), end_(ending)
{
}

const std::byte &Page::const_iterator::operator*()
{
    return *ptr_;
}

Page::const_iterator &Page::const_iterator::operator++()
{
    if (ptr_ == end_)
    {
        throw std::runtime_error("Cannot read past the page's memory.");
    }
    ++ptr_;
    return *this;
}

Page::const_iterator Page::const_iterator::operator++(int)
{
    auto tmp = *this;
    ++(*this);
    return tmp;
}

Page::const_iterator Page::const_iterator::operator+(int offset)
{
    if (ptr_ + offset >= end_)
    {
        throw std::out_of_range("Cannot read past the page's memory.");
    }
    auto tmp = *this;
    for (int i = 0; i < offset; ++i, ++tmp)
        ;
    return tmp;
}
