#include "Series.hpp"

SeriesView::SeriesView(const TapeReader::iterator begin, int records_number) : beginning_(begin), length_(records_number)
{
}
