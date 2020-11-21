#ifndef MERGER_HPP
#define MERGER_HPP

#include "Tape.hpp"
#include <vector>
#include <string_view>
#include <TapeReader.hpp>
#include <TapeWriter.hpp>
#include <utility>

class Merger
{
  public:
    Merger(std::vector<Tape>& distributed_tapes, const std::string_view output_tape_path, int page_size);
    void operator()();

  private:
    void MergeSeries(TapeReader& longer_reader, TapeReader& shorter_reader, TapeWriter& output_writer);
    void PassSeries(TapeReader& src, TapeWriter& dst);
    void MergePhase(Tape& longer, Tape& shorter, Tape& output);

    std::unique_ptr<TapeReader> longer_reader_;
    std::unique_ptr<TapeReader> shorter_reader_;
    std::unique_ptr<TapeWriter> output_writer_;
    std::vector<Tape>& distributed_tapes_;
    const std::string_view output_tape_path_;
    const int page_size_;
};

#endif // MERGER_HPP