#include <gtest/gtest.h>
#include "Merger.hpp"
#include <string>
#include <utility>
#include <TestConfig.hpp>
#include "Distributor.hpp"
#include <fmt/format.h>
#include <fstream>

class MergingTest : public testing::TestWithParam<std::pair<std::string /* path */, int /* page size */>>
{
  public:
    MergingTest()
        : input_path_(TestConfig::GetResourcePath() + GetParam().first), page_size_(GetParam().second)
    {
        fmt::print("[{:^10}] {}\n", "TAPE PATH", input_path_);
        Distributor distributor(input_path_, page_size_);
        distributed_tapes_ = distributor();

        output_path_ = TestConfig::GetTmpFilePath();
        Merger merger(distributed_tapes_, output_path_, page_size_);
        merger();
    }

  protected:
    std::string output_path_;
    std::string input_path_;
    std::vector<Tape> distributed_tapes_;
    int page_size_;
};

INSTANTIATE_TEST_CASE_P(
    MergingDistributedTapesTest, MergingTest,
    ::testing::Values(std::make_pair("no_coalescing_series_6_with_2_dummy_page_size_64.records", 64),
                      std::make_pair("01_prepared_series_8_coalescing_4_page_size_64.records", 64),
                      std::make_pair("02_prepared_series_8_coalescing_4_page_size_64.records", 64),
                      std::make_pair("prepared_series_5_records_5_page_size_64.records", 64),
                      std::make_pair("01_series_25_page_size_64.records", 64),
                      std::make_pair("01_series_60_page_size_64.records", 64),
                      std::make_pair("01_series_120_page_size_64.records", 64),
                      std::make_pair("01_series_512_page_size_4096.records", 4096),
                      std::make_pair("02_series_25_page_size_64.records", 64),
                      std::make_pair("02_series_60_page_size_64.records", 64),
                      std::make_pair("02_series_120_page_size_64.records", 64),
                      std::make_pair("02_series_512_page_size_4096.records", 4096),
                      std::make_pair("03_series_25_page_size_64.records", 64),
                      std::make_pair("03_series_60_page_size_64.records", 64),
                      std::make_pair("03_series_120_page_size_64.records", 64),
                      std::make_pair("fibonacci_series_2584_page_size_4096.records", 4096),
                      std::make_pair("fibonacci_series_4181_page_size_4096.records", 4096)));

TEST_P(MergingTest, sunny_scenario_CheckIfTapeIsSortedAfterMerging)
{
    TapeReader reader(output_path_, page_size_);
    auto iter = reader.begin();
    auto prev_record = *iter;
    ++iter;
    for (const auto& record : reader)
    {
        EXPECT_LE(prev_record, record);
        prev_record = record;
    }
}
