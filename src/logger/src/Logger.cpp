#include "logger/Logger.hpp"
#include "Logger_internal.hpp"
#include <fmt/format.h>
#include <fmt/chrono.h>

LoggerImpl logger_singleton(LoggerImpl::DEFAULT_PATH);

LoggerImpl::LoggerImpl(const std::string_view output_path) : output_stream_(output_path.data())
{
    if (!output_stream_.is_open())
    {
        throw std::runtime_error(fmt::format("Failed to open output log file at path \"{}\"", output_path));
    }
}

void LoggerImpl::Write(const std::string& message)
{
    //AppendTimestamp();
    output_stream_ << message << '\n';
}

void LoggerImpl::Write(std::string_view message)
{
    //AppendTimestamp();
    output_stream_.write(message.data(), message.size());
    output_stream_ << '\n';
}

void LoggerImpl::ChangeOutputPath(const std::string_view output_file_path)
{
    output_stream_ = std::ofstream(output_file_path.data());
    if (!output_stream_.is_open())
    {
        throw std::runtime_error(
            fmt::format("Failed to open output log file at path \"{}\"", output_file_path));
    }
}

template <typename T>
static auto to_ns(const std::chrono::time_point<T>& tp)
{
    using namespace std::chrono;

    auto dur = tp.time_since_epoch();
    return duration_cast<nanoseconds>(dur).count();
}

void LoggerImpl::AppendTimestamp()
{
    auto tp = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(tp);
    output_stream_ << fmt::format("{:%Y-%m-%d %H:%M:%S}.{} ", fmt::localtime(current_time),
                                  to_ns(tp) % 1000'000'000);
}

void Logger::SetOutputPath(const std::string& log_file_path)
{
    logger_singleton.ChangeOutputPath(log_file_path);
}

void Logger::Dump(const std::string& message) { logger_singleton.Write(message); }

void Logger::Dump(std::string_view message) { logger_singleton.Write(message); }
