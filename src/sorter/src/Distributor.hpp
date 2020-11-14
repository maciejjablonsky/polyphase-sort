#ifndef DISTRIBUTOR_HPP
#define DISTRIBUTOR_HPP

#include <string_view>
#include <vector>
#include <string>
#include "Tape.hpp"
#include <TapeWriter.hpp>
#include <TapeReader.hpp>

class Distributor
{
  public:
    Distributor(const std::string_view input_tape_file_path, const int output_tapes_number = 2);
    std::vector<Tape> operator()() const;

  private:
    std::vector<Tape> GenerateOutputTapes() const;
    void WriteSeriesToTape(TapeReader & in, const uint64_t series, TapeWriter& out);

    struct Fibonacci
    {
        int curr;
        int next;
        Fibonacci();
        int Next();
    } fibonacci;
    const std::string_view input_tape_file_path_;
    const uint64_t output_tapes_number_ = 0;
};

#endif // DISTRIBUTOR_HPP