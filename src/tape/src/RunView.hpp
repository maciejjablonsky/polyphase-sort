#ifndef RUN_VIEW_HPP
#define RUN_VIEW_HPP

#include "TapeReader.hpp"
#include "Record.hpp"

class RunView
{
  public:
    class iterator
    {
      public:
        iterator(TapeReader::iterator tape_iter);
        friend bool operator==(iterator lhs, iterator rhs);
        friend bool operator!=(iterator lhs, iterator rhs);
        iterator &operator++();

      private:
        TapeReader::iterator it;
    };
    RunView(TapeReader::iterator beginning, TapeReader::iterator ending);
    int Length() const;
    iterator begin();
    iterator end();
  private:
    TapeReader::iterator beginning_;
    TapeReader::iterator ending_;
    int length_;
};

#endif RUN_VIEW_HPP