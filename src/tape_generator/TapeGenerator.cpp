#include "TapeGenerator.hpp"
#include <charconv>
#include <fmt/format.h>
#include <iostream>
#include "Record.hpp"
#include "TapeWriter.hpp"
#include <limits>
#include <ctime>
#include <cstdlib>

template <typename T>
std::optional<T> get_optional_argument_value(const std::vector<std::string_view>& argv,
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

TapeGenerator::TapeGenerator(const int argc, const char* argv[]) : argv_{argv + 1, argv + argc}
{
    auto is_records_number = GetNumberOfRecordsToGenerate();
    auto is_out_file_path = GetOuputFilePath();
    auto is_page_size = GetPageSize();
    auto is_series_num = GetSeriesNum();
    try
    {
        if ((is_records_number == is_series_num))
        {
            throw std::runtime_error("You can use only records_number or series_number");
        }
        if (!is_out_file_path)
        {
            throw std::runtime_error("You need to provide output file path");
        }
    }
    catch (const std::exception& e)
    {
        PrintUsage();
        fmt::print("Error: {}\n", e.what());
        std::exit(-1);
    }
    if (is_page_size)
    {
        page_size_ = *is_page_size;
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

std::optional<int> TapeGenerator::GetSeriesNum() const
{
    return get_optional_argument_value<int>(argv_, TapeGenerator::SERIES_NUM_COMMAND);
}

void TapeGenerator::Generate() const
{

    if (auto opt_records = GetNumberOfRecordsToGenerate(); opt_records)
    {
        GenerateRecords(*opt_records);
    }
    else if (auto opt_series = GetSeriesNum(); opt_series)
    {
        GenerateSeries(*opt_series);
    }
}

void TapeGenerator::PrintUsage() const
{
    fmt::print("Usage: tape_generator [{} <number> | {} <number>] [{} <file path>] [{} <page size>]\n",
               RECORD_NUM_COMMAND, SERIES_NUM_COMMAND, OUTPUT_FILE_COMMAND, PAGE_SIZE_COMMAND);
}

void TapeGenerator::GenerateRecords(int records_num) const
{
    auto out_file_path = *GetOuputFilePath();
    TapeWriter writer(out_file_path, page_size_);
    for (int i = 0; i < records_num; ++i)
    {
        auto record = Record::GetRandom();
        writer.Write(Record::Serialize(record));
    }
    fmt::print("Written {} records to file {}. Page size is {}.\n", records_num, out_file_path, page_size_);
}

void TapeGenerator::GenerateSeries(int series_num) const
{
    std::srand(time(nullptr));
    auto out_file_path = *GetOuputFilePath();
    TapeWriter writer(out_file_path, page_size_);
    Record::RuntimeRecord last_series_start = {0xFF};
    for (int s = 0; s < series_num; ++s)
    {
        int records_in_series = (std::rand() % (TapeGenerator::max_records_in_series_ -
                                                TapeGenerator::min_records_in_series_ + 1)) +
                                TapeGenerator::min_records_in_series_;
        auto record = last_series_start + 0x8;
        for (int r = 0; r < records_in_series; ++r)
        {
            writer.Write(Record::Serialize(record));
            record += 0xF;
        }
        last_series_start += 0x2;
    }
    fmt::print("Written {} series to file {}. Page size is {}.\n", series_num, out_file_path, page_size_);
}
