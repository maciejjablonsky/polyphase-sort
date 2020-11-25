#include "App.hpp"
#include <optional>
#include <fmt/format.h>
#include <charconv>
#include <iostream>
#include <TapeWriter.hpp>
#include <TapeReader.hpp>
#include <Distributor.hpp>
#include <Merger.hpp>
#include <fstream>
#include <logger/Logger.hpp>

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

App::App(const int argc, const char* argv[]) : argv_{argv + 1, argv + argc}
{
    auto is_input_from_file = get_optional_argument_value<std::string_view>(argv_, App::INPUT_FILE_COMMAND);
    auto is_input_from_keyboard = get_flag_value(argv_, App::INPUT_KEYBOARD_COMMAND);
    auto is_page_size = get_optional_argument_value<int>(argv_, App::PAGE_SIZE_COMMAND);
    if (is_page_size)
    {
        page_size_ = *is_page_size;
    }
    try
    {
        if (static_cast<bool>(is_input_from_file) == is_input_from_keyboard)
        {
            throw std::runtime_error("Input must be passed from only one source.");
        }
    }
    catch (const std::exception& e)
    {
        PrintUsage();
        fmt::print("Error: {}\n", e.what());
        std::exit(-1);
    }
}

void App::PrintUsage() const
{

    fmt::print("Usage: polyphase-sort-app [{} <path> | {}] {}[{} <page size>]\n", INPUT_FILE_COMMAND,
               INPUT_KEYBOARD_COMMAND,
#ifdef ENABLE_LOGGING
               fmt::format("[{} <log output path>] ", LOG_OUTPUT_COMMAND)
#else
               ""
#endif  // ENABLE_LOGGING
                   ,
               PAGE_SIZE_COMMAND);
}

void App::Run()
{
#ifdef ENABLE_LOGGING
    auto is_log_path = get_optional_argument_value<std::string_view>(argv_, App::LOG_OUTPUT_COMMAND);
    std::string log_path = "output.log";
    if (is_log_path)
    {
        log_path = {is_log_path->data(), is_log_path->size()};
    }
    Logger::SetOutputPath(log_path);
#endif  // ENABLE_LOGGING
    std::string input_tape = GetInputPath();
    Distributor distributor(input_tape, page_size_);
    auto distributed_tapes = distributor();

    std::string_view output_path = "sorted_output_tape.records";
    Merger merger(distributed_tapes, output_path, page_size_);
    merger();
    fmt::print("Tape sorted.\n");
    if (get_flag_value(argv_, App::DISPLAY_SORTED_COMAND))
    {
        PrintTape({output_path.data(), output_path.size()});
    }

#ifdef ENABLE_LOGGING
    ParseLog(log_path);
#endif  // ENABLE_LOGGING
}

void App::ParseLog(const std::string& path)
{
    std::ifstream in_stream(path);
    if (!in_stream.is_open())
    {
        throw std::runtime_error(fmt::format("Failed to open log file at {}", path));
    }
    std::string line;

    struct LogData
    {
        int read_operations = 0;
        int write_operations = 0;
        int merging_phases = 0;
    } data;

    while (std::getline(in_stream, line))
    {
        if (line.starts_with("Page read"))
        {
            ++data.read_operations;
        }
        else if (line.starts_with("Page written"))
        {
            ++data.write_operations;
        }
        else if (line.starts_with("End of merge phase"))
        {
            ++data.merging_phases;
        }
    }
    fmt::print("Read operations: {}\nWrite operations: {}\n, Merge phases number: {}\n", data.read_operations,
               data.write_operations, data.merging_phases);
}

void App::PrintTape(const std::string& path)
{
    TapeReader tape(path, page_size_);
    for (int i = 0; const auto& record : tape)
    {
        fmt::print("{:<5} ", record.creation_time);
        if (++i % 20 == 0)
        {
            fmt::print("\n");
        }
    }
    fmt::print("\n");
}

std::string App::GetInputPath()
{
    auto is_input_from_file = get_optional_argument_value<std::string_view>(argv_, App::INPUT_FILE_COMMAND);
    if (is_input_from_file)
    {
        std::string_view path = *is_input_from_file;
        return {path.data(), path.size()};
    }
    return GetTapeFromKeyboard();
}

std::string App::GetTapeFromKeyboard()
{
    bool reading = true;
    fmt::print("Enter numbers separated with whitespace, enter \'q\' to finish reading.\n");
    std::string tmp_path = "user_provided_tape.records";
    [&]() {
        TapeWriter tape_writer(tmp_path, page_size_);
        while (reading)
        {
            std::string token;
            std::cin >> token;
            if (token.starts_with('q'))
            {
                reading = false;
                break;
            }

            Record::SerializedRecord new_record = {std::stoll(token)};
            tape_writer.Write(new_record);
        }
    }();
    fmt::print("Tape written.\n");
    PrintTape(tmp_path);
    return tmp_path;
}
