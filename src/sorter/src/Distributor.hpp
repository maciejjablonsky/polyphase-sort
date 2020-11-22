#ifndef DISTRIBUTOR_HPP
#define DISTRIBUTOR_HPP

#include <string_view>
#include <vector>
#include <string>
#include "Tape.hpp"
#include <TapeWriter.hpp>
#include <TapeReader.hpp>
#include <functional>
#include <utility>

class Distributor
{
  public:
    Distributor(const std::string_view input_tape_file_path, const int page_size, const int output_tapes_number = 2);
    std::vector<Tape> operator()() const;

  private:
    void PassSeries(TapeReader& src, TapeWriter& dst) const;
    std::vector<Tape> GenerateOutputTapes() const;
    TapeReader::const_iterator DistributeSeriesToTape(TapeReader& reader, TapeWriter& writer, Tape & out_tape) const;
    mutable struct Fibonacci
    {
        int a;
        int b;
        Fibonacci();
        int Next();
    } fibonacci;
    const std::string_view input_tape_file_path_;
    const int page_size_ = 0;
    const int output_tapes_number_;
};

#endif // DISTRIBUTOR_HPP