#include "Distributor.hpp"
#include <stdexcept>
#include <filetools.hpp>
#include <chrono>
#include <filesystem>
#include <fmt/format.h>

Distributor::Distributor(const std::string_view input_tape_file_path, const int output_tapes_number)
    : input_tape_file_path_(input_tape_file_path), output_tapes_number_(output_tapes_number)
{
}

std::vector<Tape> Distributor::operator()() const
{
    auto out_tapes = GenerateOutputTapes();

    return {};
}

std::vector<Tape> Distributor::GenerateOutputTapes() const
{
    struct out_tape
    {
        Tape operator()()
        {
            using namespace std::chrono;
            auto timestamp = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
            return {.dummy_series = 0, .series = 0, .file_path = fmt::format("tmp_out_{}.tape", timestamp)};
        }
    } generator;
    return {output_tapes_number_, generator()};
}

void Distributor::WriteSeriesToTape(TapeReader& in, const uint64_t series, TapeWriter& out)
{
}

Distributor::Fibonacci::Fibonacci() : curr(0), next(1)
{
}

int Distributor::Fibonacci::Next()
{
    if (next + curr < next)
    {
        throw std::overflow_error("Integer overflow when computing next fibonacci number.");
    }
    next += curr;
    curr = next;
    return curr;
}
