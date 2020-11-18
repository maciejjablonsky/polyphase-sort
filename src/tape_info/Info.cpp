#include "Info.hpp"
#include <cstdlib>
#include <optional>
#include <type_traits>
#include <fmt/format.h>
#include <charconv>
#include <algorithm>

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

bool get_flag_value(const std::vector<std::string_view>& argv, std::string_view command)
{
    auto command_iter = std::find(argv.begin(), argv.end(), command);
    return command_iter != argv.end();
}

Info::Info(const int argc, const char* argv[]) : argv_{argv + 1, argv + argc}
{
    auto is_count = get_flag_value(argv_, Info::COUNT_SERIES_COMMAND);
    auto is_check_sorted = get_flag_value(argv_, Info::IS_SORTED_COMMAND);
    auto is_input_file = get_optional_argument_value<std::string>(argv_, Info::INPUT_TAPE_PATH_COMMAND);
    try
    {
        if (!is_input_file)
        {
            throw std::runtime_error("No input file given.");
        }
    }
    catch (const std::exception& e)
    {
        fmt::print("ERROR: {}\n", e.what());
        PrintUsage();
        std::exit(-1);
    }
    auto is_page_size = get_optional_argument_value<int>(argv_, Info::PAGE_SIZE_COMMAND);
    if (is_page_size)
    {
        page_size_ = *is_page_size;
    }
}

void Info::PrintUsage() const
{
    fmt::print("Usage: info {} <file path> [{} {}]\n", Info::INPUT_TAPE_PATH_COMMAND,
               Info::COUNT_SERIES_COMMAND, Info::IS_SORTED_COMMAND);
}

void Info::Run()
{
    if (get_flag_value(argv_, Info::IS_SORTED_COMMAND))
    {
        IsSorted();
    }
    if (get_flag_value(argv_, Info::COUNT_SERIES_COMMAND))
    {
        CountSeries();
    }
}

void Info::IsSorted() const
{
    auto in_path = *get_optional_argument_value<std::string>(argv_, Info::INPUT_TAPE_PATH_COMMAND);
    TapeReader reader(in_path, page_size_);
    Record::RuntimeRecord last_record = {0};
    for (uint64_t i = 0; const auto& record : reader)
    {
        auto de_record = Record::Deserialize(&record);
        if (de_record < last_record)
        {
            fmt::print("Tape isn't sorted. Breaked at {} record.\n", i);
            return;
        }
        last_record = de_record;
        ++i;
    }
    fmt::print("Tape sorted.\n");
}

void Info::CountSeries() const
{
    auto in_path = *get_optional_argument_value<std::string>(argv_, Info::INPUT_TAPE_PATH_COMMAND);
    TapeReader reader(in_path, page_size_);
    uint64_t series = 1;
    Record::RuntimeRecord last_record = {0};
    for (const auto& record : reader)
    {
        auto de_record = Record::Deserialize(&record);
        if (de_record < last_record)
        {
            ++series; 
        }
        last_record = de_record;
    }
    fmt::print("Tape contains {} series.\n", series);
}
