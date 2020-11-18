#ifndef MERGER_HPP
#define MERGER_HPP

#include "Tape.hpp"
#include <vector>
#include <string_view>
#include <TapeReader.hpp>
#include <TapeWriter.hpp>

class Merger
{
  public:
    Merger(std::vector<Tape>& distributed_tapes, const std::string_view output_tape_path, int page_size);
    void operator()();

  private:
    void MergeSeries(TapeReader& lhs, TapeReader& rhs, TapeWriter& output);
    void PassSeries(TapeReader & src, TapeWriter & dst);
    void MergePhase(Tape& larger, Tape& smaller, Tape& output);
    std::vector<Tape>& distributed_tapes_;
    const std::string_view output_tape_path_;
    const int page_size_;
};

#endif // MERGER_HPP