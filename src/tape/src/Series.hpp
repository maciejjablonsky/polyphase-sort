#ifndef SERIES_HPP
#define SERIES_HPP

#include "TapeReader.hpp"
#include "Record.hpp"

class Series
{
    public:
        Series(const TapeReader::iterator begin, const TapeReader::iterator end);
    private:
        std::vector<Record::SerializedRecord> records_;
};

#endif SERIES_HPP