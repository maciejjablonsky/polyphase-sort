#ifndef SERIES_HPP
#define SERIES_HPP

#include "TapeReader.hpp"
#include "Record.hpp"

class SeriesView
{
  public:
    SeriesView(const TapeReader::iterator begin, int records_number);
    
  private:
    const TapeReader::iterator beginning_;
    int length_;
};

#endif SERIES_HPP