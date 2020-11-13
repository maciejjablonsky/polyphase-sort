#include "TapeReader.hpp"
#include <stdexcept>
#include "RecordsPage.hpp"

template <class... Ts> struct overloaded : Ts...
{
    using Ts::operator()...;
};

template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

TapeReader::TapeReader(const std::string_view in_tape_file_path, const int page_size)
    : source_(std::in_place_type<PageReader>, in_tape_file_path, page_size),
      current_page_(std::visit<Page>(overloaded{[](PageReader& reader) { return reader.ReadPage(); },
                                                [](std::queue<Page>& pages_queue) {
                                                    auto page = pages_queue.front();
                                                    pages_queue.pop();
                                                    return page;
                                                }},
                                     source_)),
      current_page_iterator_(current_page_.cbegin())
{
}

TapeReader::TapeReader(std::queue<Page>&& pages)
    : source_(std::in_place_type<std::queue<Page>>, std::move(pages)),
      current_page_(std::visit<Page>(overloaded{[](PageReader& reader) { return reader.ReadPage(); },
                                                [](std::queue<Page>& pages_queue) {
                                                    auto page = pages_queue.front();
                                                    pages_queue.pop();
                                                    return page;
                                                }},
                                     source_)),
      current_page_iterator_(current_page_.cbegin())
{
}

TapeReader::const_iterator TapeReader::cbegin()
{
    return {*this};
}

TapeReader::const_iterator TapeReader::begin() const
{
    return const_cast<TapeReader&>(*this).cbegin();
}

TapeReader::sentinel TapeReader::cend()
{
    return sentinel{};
}

TapeReader::sentinel TapeReader::end() const
{
    return const_cast<TapeReader&>(*this).cend();
}

bool TapeReader::LastPageLoaded() const
{
    return std::visit(overloaded{[](const PageReader& reader) { return reader.WasLastPageRead(); },
                                 [](const std::queue<Page>& pages_queue) { return pages_queue.empty(); }},
                      source_);
}

void TapeReader::ReadNextPage()
{
    current_page_ = RecordsPage(std::visit(overloaded{[](PageReader& reader) { return reader.ReadPage(); },
                                                      [](std::queue<Page>& pages_queue) {
                                                          auto page = pages_queue.front();
                                                          pages_queue.pop();
                                                          return page;
                                                      }},
                                           source_));
    current_page_iterator_ = current_page_.begin();
}

TapeReader::const_iterator::const_iterator(TapeReader& parent) : parent_(&parent)
{
}

TapeReader::const_iterator& TapeReader::const_iterator::operator++()
{
    ++parent_->current_page_iterator_;
    if (parent_->current_page_iterator_ == parent_->current_page_.cend())
    {
        if (!parent_->LastPageLoaded())
        {
            parent_->ReadNextPage();
        }
    }
    return *this;
}

const TapeReader::tape_item& TapeReader::const_iterator::operator*()
{
    return *parent_->current_page_iterator_;
}

const TapeReader::tape_item& TapeReader::const_iterator::operator->()
{
    return **this;
}

bool operator==(const TapeReader::const_iterator& iter, TapeReader::sentinel)
{
    return iter.parent_->LastPageLoaded() &&
           iter.parent_->current_page_iterator_ == iter.parent_->current_page_.cend();
}

bool operator!=(const TapeReader::const_iterator& iter, TapeReader::sentinel sentinel)
{
    return !(iter == sentinel);
}
