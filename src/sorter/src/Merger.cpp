#include "Merger.hpp"
#include <algorithm>
#include <chrono>
#include <fmt/format.h>
#include "OutTmpTapeGenerator.hpp"
#include <functional>
#include "Tape.hpp"
#include <filesystem>
#include <compare>

Merger::Merger(std::vector<Tape>& distributed_tapes, const std::string_view output_tape_path, int page_size)
    : distributed_tapes_(distributed_tapes), output_tape_path_(output_tape_path), page_size_(page_size)
{
}

void Merger::operator()()
{
    distributed_tapes_.push_back(out_tmp_tape()());
    std::vector<std::reference_wrapper<Tape>> tapes{distributed_tapes_.begin(), distributed_tapes_.end()};
    shorter_reader_ = std::make_unique<TapeReader>(distributed_tapes_.at(1).file_path, page_size_);
    while (std::count_if(tapes.begin(), tapes.end(), [](const Tape& tape) { return tape.series > 0; }) > 1)
    {
        std::sort(tapes.begin(), tapes.end(), std::greater<Tape>());
        longer_reader_ = std::make_unique<TapeReader>(tapes.front().get().file_path, page_size_);
        output_writer_ = std::make_unique<TapeWriter>(tapes.back().get().file_path, page_size_);
        MergePhase(tapes.at(0), tapes.at(1), tapes.back());
        shorter_reader_ = std::move(longer_reader_);
    }

    std::sort(tapes.begin(), tapes.end(), std::less_equal<Tape>());
    auto tmp_output_file_path = tapes.front().get().file_path;
    std::filesystem::rename(tmp_output_file_path.c_str(), output_tape_path_);
}

void Merger::MergeSeries(TapeReader& lhs, TapeReader& rhs, TapeWriter& output)
{
    auto lhs_iter = lhs.cbegin();
    auto rhs_iter = rhs.cbegin();
    auto last_record = *lhs_iter;
    while (lhs_iter != lhs.cend() && rhs_iter != rhs.cend())
    {
        if (*lhs_iter <= *rhs_iter)
        {
            last_record = *lhs_iter;
            ++lhs_iter;
            output.Write(last_record);
        }
        else
        {
            last_record = *rhs_iter;
            ++rhs_iter;
            output.Write(last_record);
        }
    }
    while (lhs_iter != lhs.cend() && last_record <= *lhs_iter)
    {
        last_record = *lhs_iter;
        ++lhs_iter;
        output.Write(last_record);
    }
    while (rhs_iter != rhs.cend() && last_record <= *rhs_iter)
    {
        last_record = *rhs_iter;
        ++rhs_iter;
        output.Write(last_record);
    }
}
void Merger::PassSeries(TapeReader& src, TapeWriter& dst)
{
    auto iter = src.cbegin();
    auto last_record = *iter;
    dst.Write(last_record);
    ++iter;
    while (iter != src.cend() && last_record <= *iter)
    {
        last_record = *iter;
        ++iter;
        dst.Write(last_record);
    }
}

void Merger::MergePhase(Tape& longer, Tape& shorter, Tape& output)
{
    while (longer.dummy_series > 0)
    {
        PassSeries(*shorter_reader_, *output_writer_);
        --shorter.series;
        --longer.dummy_series;
        --longer.series;
        ++output.series;
    }

    while (shorter_reader_->cbegin() != shorter_reader_->cend())
    {
        MergeSeries(*longer_reader_, *shorter_reader_, *output_writer_);
        --shorter.series;
        --longer.series;
        ++output.series;
    }
}
