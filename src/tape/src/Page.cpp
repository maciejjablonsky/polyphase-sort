#include "Page.hpp"
#include <algorithm>

Page::Page(int page_size) : memory_(std::max(page_size, static_cast<int>(sizeof(PageHeader))))
{
    header_ = reinterpret_cast<PageHeader *>(memory_.data());
}

Page::Page(byte_vector &&memory) : memory_(std::move(memory))
{
    header_ = reinterpret_cast<PageHeader *>(memory_.data());
}
Page::Page(std::initializer_list<std::byte> list) : memory_(list)
{
    header_ = reinterpret_cast<PageHeader *>(memory_.data());
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

int Page::records_number() const
{
    return header_->inserted_records_number;
}
