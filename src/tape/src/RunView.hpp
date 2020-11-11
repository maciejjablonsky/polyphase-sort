#ifndef RUN_VIEW_HPP
#define RUN_VIEW_HPP

#include "TapeReader.hpp"
#include "Record.hpp"

class RunView
{
  public:
    RunView(const TapeReader::iterator begin, int records_number);
    int Length() const;
    TapeReader::iterator begin();
  private:
    const TapeReader::iterator beginning_;
    int length_;
};

#endif RUN_VIEW_HPP