#include "SeriesView.hpp"

RunView::RunView(const TapeReader::iterator begin, int records_number) : beginning_(begin), length_(records_number)
{
}

int RunView::Length() const
{
    return length_;
}

auto RunView::begin() const
{
    return beginning_;
}

