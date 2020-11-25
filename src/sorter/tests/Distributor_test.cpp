#include <gtest/gtest.h>
#include "Distributor.hpp"
#include "TestConfig.hpp"
#include <algorithm>
#include <string_view>
#include <string>
#include <fmt/format.h>
#include <cstring>
#include <logger/Logger.hpp>

using namespace std::string_literals;

namespace
{
std::string replace(std::string& str, const std::string& from, const std::string& to)
{
    std::string copy = str;
    size_t start_pos = copy.find(from);
    if (start_pos == std::string::npos)
    {
        return copy;
    }
    copy.replace(start_pos, from.length(), to);
    return copy;
}
}  // namespace

struct DistributionTestParams
{
    std::string path;
    int page_size;
    std::vector<int> expected_series_numbers;

    friend std::ostream& operator<<(std::ostream& os, const DistributionTestParams& params)
    {
        os << fmt::format("({{path: {}}}, {{page_size: {}}}, {{expected_series_numbers: ", params.path,
                          params.page_size);
        os << '{';
        for (int i = 0; const auto num : params.expected_series_numbers)
        {
            ++i;
            os << fmt::format("{}{}", num, (i < params.expected_series_numbers.size() ? ", " : ""));
        }
        os << "}})\n";
        return os;
    }
};

class DistributionTest : public ::testing::TestWithParam<DistributionTestParams>
{
   public:
    DistributionTest()
        : tape_path_(TestConfig::GetResourcePath() + GetParam().path),
          page_size_(GetParam().page_size),
          expected_series_numbers_(GetParam().expected_series_numbers)
    {
        Logger::SetOutputPath(replace(TestConfig::GetTmpDirPath() + GetParam().path, ".records", ".log"));
        fmt::print("[{:^10}] {}\n", "TAPE PATH", tape_path_);
    }

   protected:
    const std::string tape_path_;
    const int page_size_;
    const std::vector<int> expected_series_numbers_;
};

INSTANTIATE_TEST_SUITE_P(
    DistributionExpectedTapesOutputTest, DistributionTest,
    ::testing::Values(
        DistributionTestParams{"no_coalescing_series_6_with_2_dummy_page_size_64.records", 64, {5, 3}},
        DistributionTestParams{"01_prepared_series_8_coalescing_4_page_size_64.records", 64, {5, 3}},
        DistributionTestParams{"02_prepared_series_8_coalescing_4_page_size_64.records", 64, {5, 3}},
        DistributionTestParams{"prepared_series_5_records_5_page_size_64.records", 64, {3, 2}},
        DistributionTestParams{"01_series_25_page_size_64.records", 64, {21, 13}},
        DistributionTestParams{"01_series_60_page_size_64.records", 64, {55, 34}},
        DistributionTestParams{"01_series_120_page_size_64.records", 64, {89, 55}},
        DistributionTestParams{"01_series_512_page_size_4096.records", 4096, {377, 233}},
        DistributionTestParams{"02_series_25_page_size_64.records", 64, {21, 13}},
        DistributionTestParams{"02_series_60_page_size_64.records", 64, {55, 34}},
        DistributionTestParams{"02_series_120_page_size_64.records", 64, {89, 55}},
        DistributionTestParams{"02_series_512_page_size_4096.records", 4096, {377, 233}},
        DistributionTestParams{"03_series_25_page_size_64.records", 64, {21, 13}},
        DistributionTestParams{"03_series_60_page_size_64.records", 64, {55, 34}},
        DistributionTestParams{"03_series_120_page_size_64.records", 64, {89, 55}},
        DistributionTestParams{"fibonacci_series_2584_page_size_4096.records", 4096, {1597, 987}},
        DistributionTestParams{"fibonacci_series_4181_page_size_4096.records", 4096, {2584, 1597}}));

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
        : tape_path_(TestConfig::GetResourcePath() + GetParam().path),
          page_size_(GetParam().page_size),
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

TEST(TapesDistributionTest, CheckIfCreatedEqualToPrepared)
{
    const std::string input_tape_path =
        TestConfig::GetResourcePath() + "descending_series_page_size_64.records";
    Distributor distributor(input_tape_path, 64);
    auto tapes = distributor();
    {
        TapeReader actual_reader(tapes[0].file_path, 64);
        const std::string expected_tape_path =
            TestConfig::GetResourcePath() + "longer_distributed_tape_page_size_64.records";
        TapeReader expected_reader(expected_tape_path, 64);
        auto actual = actual_reader.cbegin();
        auto expected = expected_reader.cbegin();
        while (expected != expected_reader.cend())
        {
            EXPECT_EQ(*actual, *expected);
            ++actual;
            ++expected;
        }
    }
    {
        TapeReader actual_reader(tapes[1].file_path, 64);
        const std::string expected_tape_path =
            TestConfig::GetResourcePath() + "shorter_distributed_tape_page_size_64.records";
        TapeReader expected_reader(expected_tape_path, 64);
        auto actual = actual_reader.cbegin();
        auto expected = expected_reader.cbegin();
        while (expected != expected_reader.cend())
        {
            EXPECT_EQ(*actual, *expected);
            ++actual;
            ++expected;
        }
    }
}
