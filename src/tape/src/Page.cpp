#include "Page.hpp"
#include <algorithm>
#include <stdexcept>
#include "TapeReader.hpp"

Page::Page() : memory_(DEFAULT_PAGE_SIZE)
{
}

Page::Page(uint64_t page_size) : memory_(page_size)
{
}

//Page::Page(byte_vector&& memory) : memory_(std::move(memory))
//{
//}
Page::Page(std::initializer_list<std::byte> list) : memory_(list)
{
}

int Page::size() const
{
    return memory_.size();
}

std::byte* Page::data()
{
    return memory_.data();
}

const std::byte* Page::data() const
{
    return memory_.data();
}

Page::iterator Page::begin()
{
    return { memory_.data(), memory_.size()};
}

Page::iterator Page::end()
{
return {memory_.data() + memory_.size(), memory_.size()}; 
}

Page::const_iterator Page::cbegin()
{
    return {memory_.data(), memory_.size()};
}

Page::const_iterator Page::begin() const
{
    return const_cast<Page&>(*this).cbegin();
}

Page::const_iterator Page::cend()
{
    return {memory_.data() + memory_.size(), memory_.size()};
}

Page::const_iterator Page::end() const
{
    return const_cast<Page&>(*this).cend();
}

bool operator==(const Page::const_iterator& lhs, const Page::const_iterator& rhs)
{
    return lhs.ptr_ == rhs.ptr_;
}

bool operator!=(const Page::const_iterator& lhs, const Page::const_iterator& rhs)
{
    return !(lhs.ptr_ == rhs.ptr_);
}

Page::const_iterator::const_iterator(const std::byte* ptr, const uint64_t size) : ptr_(ptr), page_size_(size)
{
}

const std::byte& Page::const_iterator::operator*()
{
    return *ptr_;
}

Page::const_iterator& Page::const_iterator::operator++()
{
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
    auto tmp = *this;
    tmp.ptr_ += offset;
    return tmp;
}

void Page::const_iterator::operator+=(uint64_t offset)
{
    *this = *this + offset;
}

Page::const_iterator Page::const_iterator::operator-(uint64_t offset)
{
    auto tmp = *this;
    tmp.ptr_ -= offset;
    return tmp;
}

Page::iterator::iterator(std::byte* ptr, const uint64_t size) : ptr_(ptr), page_size_(size)
{
}

std::byte& Page::iterator::operator*()
{
    return *ptr_;
}

Page::iterator& Page::iterator::operator++()
{
    ++ptr_;
    return *this;
}

Page::iterator Page::iterator::operator++(int)
{
    auto tmp = *this;
    ++(*this);
    return tmp;
}

Page::iterator Page::iterator::operator+(uint64_t offset)
{
    auto tmp = *this;
    tmp.ptr_ += offset;
    return tmp;
}

void Page::iterator::operator+=(uint64_t offset)
{
    *this = *this + offset;
}

Page::iterator Page::iterator::operator-(uint64_t offset)
{
    auto tmp = *this;
    tmp.ptr_ -= offset;
    return tmp;
}

bool operator==(const Page::iterator& lhs, const Page::iterator& rhs)
{
    return lhs.ptr_ == rhs.ptr_;
}

bool operator!=(const Page::iterator& lhs, const Page::iterator& rhs)
{
    return lhs.ptr_ != rhs.ptr_;
}
