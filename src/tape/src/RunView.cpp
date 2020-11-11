#include "RunView.hpp"

RunView::RunView(TapeReader::iterator beginning, TapeReader::iterator ending)
    : beginning_(beginning), ending_(ending), length_(ending - beginning + 1)
{
}

int RunView::Length() const
{
    return length_;
}

RunView::iterator RunView::begin()
{
    return {beginning_};
}

RunView::iterator RunView::end()
{
    return {ending_};
}

RunView::iterator::iterator(TapeReader::iterator tape_iter) : tape_iter_(tape_iter)
{
}


bool operator==(RunView::iterator lhs, RunView::iterator rhs)
{
    return lhs.tape_iter_ == rhs.tape_iter_;
}

bool operator!=(RunView::iterator lhs, RunView::iterator rhs)
{
    return lhs.tape_iter_ != rhs.tape_iter_;
}

RunView::iterator &RunView::iterator::operator++()
{
    ++tape_iter_;
    return *this;
}


decltype(TapeReader::iterator::operator*) & RunView::iterator::operator*()
{
    return *tape_iter_;
}
