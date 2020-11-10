#ifndef SERIES_HPP
#define SERIES_HPP

#include "TapeReader.hpp"
#include "Record.hpp"

class SeriesView
{
  public:
    struct sentinel
    {
    };
    class iterator : TapeReader::iterator
    {
      public:
        friend ;
        friend bool operator!=(const iterator &iter, sentinel);

      private:
          int 
    };
    SeriesView(const TapeReader::iterator begin, int records_number);
    int Length() const;
    auto begin() const;
    
  private:
    const TapeReader::iterator beginning_;
    int length_;
};

#endif SERIES_HPP