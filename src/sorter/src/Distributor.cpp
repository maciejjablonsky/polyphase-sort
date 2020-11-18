#include "Distributor.hpp"
#include <stdexcept>
#include <filetools.hpp>
#include <chrono>
#include <filesystem>
#include <fmt/format.h>
#include <functional>
#include "Record.hpp"

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
    : input_tape_file_path_(input_tape_file_path), page_size_(page_size),
      output_tapes_number_(output_tapes_number)
{
}

std::vector<Tape> Distributor::operator()() const
{
    auto out_tapes = GenerateOutputTapes();
    TapeReader reader(input_tape_file_path_, page_size_);

    auto writers = make_vector(out_tapes, [&](const Tape& tape) -> TapeWriter {
        return {tape.file_path, page_size_};
    });

    decltype(out_tapes.size()) current_out_tape_idx = 0;
    while (!reader.Empty())
    {
        if (WriteSeriesToTape(reader, writers[current_out_tape_idx], out_tapes[current_out_tape_idx]) !=
            reader.cend())
        {
            current_out_tape_idx = std::modulus()(current_out_tape_idx + 1, out_tapes.size());
        }
    }

    std::sort(out_tapes.begin(), out_tapes.end(), std::greater<>());
    return out_tapes;
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
    return {static_cast<size_t>(output_tapes_number_), generator()};
}

TapeReader::const_iterator Distributor::WriteSeriesToTape(TapeReader& reader, TapeWriter& writer,
                                                          Tape& out_tape) const
{
    auto reader_it = reader.cbegin();
    auto last_record = writer.LastWrittenRecord();
    int coalescing = static_cast<int>(last_record <= *reader_it);
    auto series_to_write = fibonacci.Next() - out_tape.series + coalescing;
    int series_written = 0;
    while (reader_it != reader.cend() && series_written < series_to_write)
    {
        ++series_written;
        do
        {
            writer.Write(*reader_it);
            last_record = *reader_it;
            ++reader_it;

        } while (reader_it != reader.cend() && last_record <= *reader_it);
    }
    out_tape.series += series_to_write - coalescing;
    out_tape.dummy_series = series_to_write - series_written;
    return reader_it;
}

Distributor::Fibonacci::Fibonacci() : a(0), b(1)
{
}

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
