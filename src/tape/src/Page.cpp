#include "Page.hpp"
#include <algorithm>
#include <stdexcept>

Page::Page()
    : memory_(sizeof(PageHeader)), header_(reinterpret_cast<PageHeader *>(memory_.data()))
{
}

Page::Page(int page_size)
    : memory_(std::max(page_size, static_cast<int>(sizeof(PageHeader)))),
      header_(reinterpret_cast<PageHeader *>(memory_.data())) 
{
}

Page::Page(byte_vector &&memory)
   : memory_(std::move(memory)), header_(reinterpret_cast<PageHeader *>(memory_.data()))
{
}
Page::Page(std::initializer_list<std::byte> list)
    : memory_(list), header_(reinterpret_cast<PageHeader *>(memory_.data()))
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
    return {memory_.data(), *this};
}

Page::iterator Page::end()
{
    return {memory_.data() + memory_.size(), *this};
}

bool operator==(Page::iterator lhs, Page::iterator rhs)
{
    return lhs.ptr_ == rhs.ptr_;
}

bool operator!=(Page::iterator lhs, Page::iterator rhs)
{
    return !(lhs == rhs);
}

Page::iterator::iterator(std::byte *ptr, Page &parent) : ptr_(ptr), page_(parent)
{
}

Page::iterator &Page::iterator::operator++()
{
    if (*this == page_.end())
    {
        throw std::runtime_error("Cannot read past the page's memory.");
    }
    ++ptr_;
    return *this;
}

Page::iterator Page::iterator::operator+(int offset)
{
    iterator tmp = *this;
    for (int i = 0; i < offset; ++i, ++tmp)
        ;
    return tmp;
}
