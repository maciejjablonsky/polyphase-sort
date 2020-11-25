#include "Distributor.hpp"
#include <stdexcept>
#include <filetools.hpp>
#include <chrono>
#include <filesystem>
#include <fmt/format.h>
#include <functional>
#include "Record.hpp"
#include "OutTmpTapeGenerator.hpp"
#include <compare>
#include <logger/Logger.hpp>

using namespace std::string_view_literals;


template <typename Container, typename Func>
auto make_vector(Container&& container, Func&& func) -> std::vector<decltype(func(*container.begin()))>
{
    std::vector<decltype(func(*container.begin()))> tmp;
    tmp.reserve(container.size());
    for (const auto& o : container)
    {
        tmp.push_back(func(o));
    }
    return tmp;
}

Distributor::Distributor(const std::string_view input_tape_file_path, const int page_size,
                         const int output_tapes_number)
    : input_tape_file_path_(input_tape_file_path),
      page_size_(page_size),
      output_tapes_number_(output_tapes_number)
{
}

std::vector<Tape> Distributor::operator()() const
{
#ifdef ENABLE_LOGGING
    Logger::Dump(fmt::format("Started distribution of tape {}", input_tape_file_path_));
#endif  // ENABLED_LOGGING
    auto out_tapes = GenerateOutputTapes();
    TapeReader reader(input_tape_file_path_, page_size_);

    auto writers = make_vector(out_tapes, [&](const Tape& tape) -> TapeWriter {
        return {tape.file_path, page_size_};
    });

    decltype(out_tapes.size()) current_out_tape_idx = 0;
    while (!reader.Empty())
    {
        if (DistributeSeriesToTape(reader, writers[current_out_tape_idx], out_tapes[current_out_tape_idx]) !=
            reader.cend())
        {
            current_out_tape_idx = std::modulus()(current_out_tape_idx + 1, out_tapes.size());
        }
    }

    std::sort(out_tapes.begin(), out_tapes.end(), std::greater<>());

#ifdef ENABLE_LOGGING
    Logger::Dump(fmt::format("End of distribution of tape {}", input_tape_file_path_));
#endif  // ENABLED_LOGGING

    return out_tapes;
}

void Distributor::PassSeries(TapeReader& src, TapeWriter& dst) const
{
    auto src_it = src.cbegin();
    auto last_record = dst.LastWrittenRecord();
    do
    {
        last_record = *src_it;
        ++src_it;
        dst.Write(last_record);
    } while (src_it != src.end() && last_record <= *src_it);
}

std::vector<Tape> Distributor::GenerateOutputTapes() const
{
    std::vector<Tape> tapes(output_tapes_number_);
    out_tmp_tape generator;
    for (int i = 0; i < output_tapes_number_; ++i)
    {
        tapes[i] = generator();
    }
    return tapes;
}

TapeReader::const_iterator Distributor::DistributeSeriesToTape(TapeReader& reader, TapeWriter& writer,
                                                               Tape& out_tape) const
{
    auto last_record = writer.LastWrittenRecord();
    auto reader_it = reader.cbegin();
    // pass additional series if tape is coalescing
    if (last_record <= *reader_it)
    {
        PassSeries(reader, writer);
    }
    auto series_to_write = fibonacci.Next() - out_tape.series;
    int series_written = 0;
    while (reader_it != reader.cend() && series_written < series_to_write)
    {
        ++series_written;
        PassSeries(reader, writer);
    }
    out_tape.series += series_to_write;
    out_tape.dummy_series = series_to_write - series_written;
    return reader_it;
}

Distributor::Fibonacci::Fibonacci() : a(0), b(1) {}

int Distributor::Fibonacci::Next()
{
    if (a + b < a)
    {
        throw std::overflow_error(
            fmt::format("Integer overflow when computing next fibonacci number. a = {}, b = {}\n", a, b));
    }
    b += a;
    a = b - a;
    return a;
}
