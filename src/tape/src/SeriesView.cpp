#include "Series.hpp"
#include "SeriesView.hpp"

SeriesView::SeriesView(const TapeReader::iterator begin, int records_number) : beginning_(begin), length_(records_number)
{
}

int SeriesView::Length() const
{
    return length_;
}

auto SeriesView::begin() const
{
    return beginning_;
}

