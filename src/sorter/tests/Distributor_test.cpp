#include <gtest/gtest.h>
#include "Distributor.hpp"
#include "TestConfig.hpp"
#include <algorithm>
#include <string_view>
#include <string>
#include <fmt/format.h>
#include <cstring>

using namespace std::string_literals;

struct DataDefinition
{
    std::string path;
    int page_size;
    std::vector<int> expected_series_numbers;
};

class DistributionTest : public ::testing::TestWithParam<DataDefinition>
{
  public:
    DistributionTest()
        : tape_path_(TestConfig::GetResourcePath() + GetParam().path), page_size_(GetParam().page_size),
          expected_series_numbers_(GetParam().expected_series_numbers)
    {
        fmt::print("[{:^10}] {}\n", "TAPE PATH", tape_path_);
    }

  protected:
    const std::string tape_path_;
    const int page_size_;
    const std::vector<int> expected_series_numbers_;
};

INSTANTIATE_TEST_CASE_P(DistributionExpectedTapesOutputTest, DistributionTest,
                        ::testing::ValuesIn(std::vector<DataDefinition>{
                            {"no_coalescing_series_6_with_2_dummy_page_size_64.records", 64, {5, 3}},
                            {"01_prepared_series_8_coalescing_4_page_size_64.records", 64, {5, 3}},
                            {"02_prepared_series_8_coalescing_4_page_size_64.records", 64, {5, 3}},
                            {"prepared_series_5_records_5_page_size_64.records", 64, {3, 2}},
                            {"01_series_25_page_size_64.records", 64, {21, 13}},
                            {"01_series_60_page_size_64.records", 64, {55, 34}},
                            {"01_series_120_page_size_64.records", 64, {89, 55}},
                            {"01_series_512_page_size_4096.records", 4096, {377, 233}},
                            {"02_series_25_page_size_64.records", 64, {21, 13}},
                            {"02_series_60_page_size_64.records", 64, {55, 34}},
                            {"02_series_120_page_size_64.records", 64, {89, 55}},
                            {"02_series_512_page_size_4096.records", 4096, {377, 233}},
                            {"03_series_25_page_size_64.records", 64, {21, 13}},
                            {"03_series_60_page_size_64.records", 64, {55, 34}},
                            {"03_series_120_page_size_64.records", 64, {89, 55}},
                            {"fibonacci_series_2584_page_size_4096.records", 4096, {1597, 987}},
                            {"fibonacci_series_4181_page_size_4096.records", 4096, {2584, 1597}}}));

TEST_P(DistributionTest, sunny_scenario_ReturnTapesInDescendingOrder)
{
    Distributor distributor(tape_path_, page_size_);
    auto distributed_tapes = distributor();
    EXPECT_TRUE(std::is_sorted(distributed_tapes.begin(), distributed_tapes.end(), std::greater<Tape>()));
}

TEST_P(DistributionTest, sunny_scenario_TapesHaveExpectedSeriesNumber)
{
    Distributor distributor(tape_path_, page_size_);
    auto distributed_tapes = distributor();
    for (int i = 0; i < expected_series_numbers_.size(); ++i)
    {
        EXPECT_EQ(distributed_tapes[i].series, expected_series_numbers_[i]);
    }
}

struct DummySeriesDataDefinition
{
    std::string path;
    int page_size;
    std::vector<int> expected_dummy_series;
};

class DummySeriesDistributionTest : public ::testing::TestWithParam<DummySeriesDataDefinition>
{
  public:
    DummySeriesDistributionTest()
        : tape_path_(TestConfig::GetResourcePath() + GetParam().path), page_size_(GetParam().page_size),
          expected_dummy_series_(GetParam().expected_dummy_series)
    {
        fmt::print("[{:^10}] {}\n", "TAPE PATH", tape_path_);
    }

  protected:
    const std::string tape_path_;
    const int page_size_;
    const std::vector<int> expected_dummy_series_;
};

INSTANTIATE_TEST_CASE_P(DummySeriesTest, DummySeriesDistributionTest,
                        ::testing::ValuesIn(std::vector<DummySeriesDataDefinition>{
                            {"01_prepared_series_8_coalescing_4_page_size_64.records", 64, {2, 0}},
                            {"02_prepared_series_8_coalescing_4_page_size_64.records", 64, {2, 0}},
                            {"no_coalescing_series_6_with_2_dummy_page_size_64.records", 64, {2, 0}}}));

TEST_P(DummySeriesDistributionTest, sunny_scenario_TapesHaveExpectedDummySeriesNumber)
{
    Distributor distributor(tape_path_, page_size_);
    auto distributed_tapes = distributor();
    for (int i = 0; i < expected_dummy_series_.size(); ++i)
    {
        EXPECT_EQ(distributed_tapes[i].dummy_series, expected_dummy_series_[i]);
    }
}
