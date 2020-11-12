#include "TapeGenerator.hpp"
#include <charconv>
#include <fmt/format.h>
#include <iostream>
#include "Record.hpp"
#include "TapeWriter.hpp"

template <typename T>
std::optional<T> get_optional_argument_value(const std::vector<std::string_view> &argv,
                                             std::string_view command)
{
    auto command_iter = std::find(argv.begin(), argv.end(), command);
    if (command_iter == argv.end() || command_iter + 1 == argv.end())
    {
        return std::nullopt;
    }
    auto command_value = *(command_iter + 1);

    T value;
    if constexpr (std::is_arithmetic<T>())
    {
        std::from_chars(command_value.data(), command_value.data() + command_value.size(), value);
    }
    else
    {
        value = command_value; 
    }
    return std::make_optional(value);
}

TapeGenerator::TapeGenerator(const int argc, const char *argv[]) : argv_{argv + 1, argv + argc}
{
    auto records_number = GetNumberOfRecordsToGenerate();
    auto out_file_path = GetOuputFilePath();
    if (!records_number || !out_file_path)
    {
        fmt::print("Usage: tape_generator {} <number> [{} <file path>] [{} <page size>]\n",
                   RECORD_NUM_COMMAND, OUTPUT_FILE_COMMAND, PAGE_SIZE_COMMAND);
        throw std::runtime_error("Invalid arguments.");
    }
    number_of_records_ = *records_number;
    out_file_path_ = *out_file_path;

    auto page_size = GetPageSize();
    if (page_size)
    {
        page_size_ = *page_size;
    }
}

std::optional<int> TapeGenerator::GetNumberOfRecordsToGenerate() const
{
    return get_optional_argument_value<int>(argv_, TapeGenerator::RECORD_NUM_COMMAND);
}

std::optional<std::string> TapeGenerator::GetOuputFilePath() const
{
    return get_optional_argument_value<std::string>(argv_, TapeGenerator::OUTPUT_FILE_COMMAND);
}

std::optional<int> TapeGenerator::GetPageSize() const
{
    return get_optional_argument_value<int>(argv_, TapeGenerator::PAGE_SIZE_COMMAND);
}

void TapeGenerator::Generate() const
{
    TapeWriter writer(out_file_path_, page_size_);
    for (int i = 0; i < number_of_records_; ++i)
    {
        auto record = Record::GetRandom();
        writer.Write(Record::Serialize(record));
        if (i % 500000 == 0)
        {
            writer.Flush(); 
        }
    }
    fmt::print("Written {} records to file {}. Page size is {}.\n", number_of_records_, out_file_path_,
               page_size_);
}