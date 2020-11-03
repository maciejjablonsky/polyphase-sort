#include "TapeGenerator.hpp"
#include <charconv>
#include <fmt/format.h>
#include <iostream>
TapeGenerator::TapeGenerator(const int argc, const char *argv[]) : argv_{argv + 1, argv + argc}
{
    auto records_number = GetNumberOfRecordsToGenerate();
    auto out_file_path = GetOuputFilePath();
    if (records_number && out_file_path)
    {
        fmt::print("Generating tape with {} records in \"{}\" file.\n", *records_number, *out_file_path);
    }
    else
    {
        fmt::print("Usage: tape_generator {} <number> [{} <file path>]\n", RECORD_NUM_COMMAND, OUTPUT_FILE_COMMAND);
    }
}

std::optional<int> TapeGenerator::GetNumberOfRecordsToGenerate() const
{
    auto command_iter = std::find(argv_.begin(), argv_.end(), TapeGenerator::RECORD_NUM_COMMAND);
    if (command_iter == argv_.end() || command_iter + 1 == argv_.end())
    {
        return std::nullopt;
    }
    auto command_value = *(command_iter + 1);
    int number_of_records = 0;
    std::from_chars(command_value.data(), command_value.data() + command_value.size(), number_of_records);
    return std::make_optional(number_of_records);
}

std::optional<std::string_view> TapeGenerator::GetOuputFilePath() const
{
    auto command_iter = std::find(argv_.begin(), argv_.end(), TapeGenerator::OUTPUT_FILE_COMMAND);
    if (command_iter == argv_.end() || command_iter + 1 == argv_.end())
    {
        return std::nullopt;
    }
    return std::make_optional(*(command_iter + 1));
}