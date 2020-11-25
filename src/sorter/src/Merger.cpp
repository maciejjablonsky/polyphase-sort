#include "Merger.hpp"
#include <algorithm>
#include <chrono>
#include <fmt/format.h>
#include "OutTmpTapeGenerator.hpp"
#include <functional>
#include "Tape.hpp"
#include <filesystem>
#include <compare>
#include <logger/Logger.hpp>
#include "Record.hpp"
using namespace std::string_view_literals;

Merger::Merger(std::vector<Tape>& distributed_tapes, const std::string_view output_tape_path, int page_size)
    : distributed_tapes_(distributed_tapes), output_tape_path_(output_tape_path), page_size_(page_size)
{
}

void Merger::operator()()
{
    distributed_tapes_.push_back(out_tmp_tape()());
    std::vector<std::reference_wrapper<Tape>> tapes{distributed_tapes_.begin(), distributed_tapes_.end()};
    shorter_reader_ = std::make_unique<TapeReader>(distributed_tapes_.at(1).file_path, page_size_);
#ifdef ENABLE_PRINT
    int phase_number = 0;
#endif  // ENABLE_PRINT
    while (std::count_if(tapes.begin(), tapes.end(), [](const Tape& tape) { return tape.series > 0; }) > 1)
    {
#ifdef ENABLE_LOGGING
        Logger::Dump("Start of merge phase."sv);
#endif  // ENABLE_LOGGING

        std::reverse(tapes.begin(), tapes.end());
        std::sort(tapes.begin(), tapes.end(), std::greater<Tape>());
        longer_reader_ = std::make_unique<TapeReader>(tapes.front().get().file_path, page_size_);
        MergePhase(tapes.at(0), tapes.at(1), tapes.back());
        shorter_reader_ = std::move(longer_reader_);

#ifdef ENABLE_PRINT
        [&]() {
            TapeReader tape(tapes.back().get().file_path, page_size_);
            fmt::print("Phase number {:02} | Tape content:\n", phase_number++);
            for (int i = 0; const auto& record : tape)
            {
                fmt::print("{:<5} ", record.creation_time);
                if (++i % 20 == 0)
                {
                    fmt::print("\n");
                }
            }
            fmt::print("\n");
        }();
#endif  // ENABLE_PRINT
#ifdef ENABLE_LOGGING
        Logger::Dump("End of merge phase."sv);
#endif  // ENABLE_LOGGING
    }

    std::sort(tapes.begin(), tapes.end(), std::greater<Tape>());
    auto tmp_output_file_path = tapes.front().get().file_path;
    std::filesystem::rename(tmp_output_file_path.c_str(), output_tape_path_);
}

void Merger::MergeOneSeries(TapeReader& lhs_reader, TapeReader& rhs_reader, TapeWriter& output_writer)
{
    auto lhs_it = lhs_reader.cbegin();
    auto rhs_it = rhs_reader.cbegin();
    Record::SerializedRecord last_lhs_record = Record::DEFAULT_MIN;
    Record::SerializedRecord last_rhs_record = Record::DEFAULT_MIN;
    enum class series_state
    {
        IN,
        END
    };
    auto lhs_state =
        (lhs_it != lhs_reader.cend() && last_lhs_record <= *lhs_it) ? series_state::IN : series_state::END;
    auto rhs_state =
        (rhs_it != rhs_reader.cend() && last_rhs_record <= *rhs_it) ? series_state::IN : series_state::END;

    while (true)
    {
        if (lhs_state == series_state::END && rhs_state == series_state::IN)
        {
            PassOneSeries(rhs_reader, output_writer);
            break;
        }
        else if (rhs_state == series_state::END && lhs_state == series_state::IN)
        {
            PassOneSeries(lhs_reader, output_writer);
            break;
        }
        else
        {
            if (*lhs_it <= *rhs_it)

            {
                last_lhs_record = *lhs_it;
                ++lhs_it;
                output_writer.Write(last_lhs_record);
            }
            else
            {
                last_rhs_record = *rhs_it;
                ++rhs_it;
                output_writer.Write(last_rhs_record);
            }
        }
        lhs_state = (lhs_it != lhs_reader.cend() && last_lhs_record <= *lhs_it) ? series_state::IN
                                                                                : series_state::END;
        rhs_state = (rhs_it != rhs_reader.cend() && last_rhs_record <= *rhs_it) ? series_state::IN
                                                                                : series_state::END;
    }
}
void Merger::PassOneSeries(TapeReader& src, TapeWriter& dst)
{
    auto src_it = src.cbegin();
    auto last_record = dst.LastWrittenRecord();
    do
    {
        last_record = *src_it;
        ++src_it;
        dst.Write(last_record);
    } while (src_it != src.cend() && last_record <= *src_it);
}

void Merger::MergePhase(Tape& longer, Tape& shorter, Tape& output)
{
    TapeWriter output_writer(output.file_path, page_size_);
    while (longer.dummy_series > 0)
    {
        PassOneSeries(*shorter_reader_, output_writer);
        --shorter.series;
        --longer.dummy_series;
        --longer.series;
        ++output.series;
    }

    while (shorter_reader_->cbegin() != shorter_reader_->cend())
    {
        MergeOneSeries(*longer_reader_, *shorter_reader_, output_writer);
        --shorter.series;
        --longer.series;
        ++output.series;
    }
}
