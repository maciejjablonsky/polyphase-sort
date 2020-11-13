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

std::vector<std::string> Distributor::operator()() const
{
    auto output_paths = GenerateOutputTapesPaths();
}

std::vector<std::string> Distributor::GenerateOutputTapesPaths() const
{
    struct tmp_path
    {
        std::filesystem::path path;
        tmp_path() : path(std::filesystem::temp_directory_path())
        {
        }
        std::string operator()()
        {
            using namespace std::chrono;
            auto timestamp = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
            return fmt::format("{}/{}.tape", path, timestamp);
        }
    } generator;
    return {output_tapes_number_, generator()};
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
