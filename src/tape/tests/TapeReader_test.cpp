#include <gtest/gtest.h>
#include "TapeReader.hpp"
#include "TestConfig.hpp"
#include <fmt/format.h>
#include <cstring>
#include <algorithm>
TEST(TapeReaderTest, sunny_scenario_EndOfTapeReached)
{
    const std::string tape_path = TestConfig::GetResourcePath() + "page_size_40_pages_2_records_7.records";
    TapeReader reader(tape_path, 40);
    int counter = 0;
    for (auto &record : reader)
    {
        counter++;
    }
    EXPECT_EQ(counter, 7);
    EXPECT_TRUE(reader.WholeTapeRead());
}

TEST(TapeReaderTest, sunny_scenario_RecordsAreReadProperly)
{
    const std::string tape_path =
        TestConfig::GetResourcePath() + "page_size_40_pages_2_records_7.records";
    TapeReader reader(tape_path, 40);
    std::vector<Record::SerializedRecord> references = {{1}, {2}, {3}, {4}, {5}, {6}, {7}};
    int counter = 0;
    for (auto & record: reader)
    {
        EXPECT_EQ(record, references.at(counter));
        counter++;
    }
    EXPECT_EQ(counter, 7);
}

TEST(TapeReaderIteratorsTest, DISABLED_sunny_scenario_CopyAllRecords)
{
    const std::string tape_path =
        TestConfig::GetResourcePath() + "page_size_40_pages_2_records_7.records";
    TapeReader reader(tape_path, 40);
    std::vector<Record::SerializedRecord> references = {{1}, {2}, {3}, {4}, {5}, {6}, {7}};
    std::vector<Record::SerializedRecord> copied;
    std::copy_n(references.begin(), references.size(), copied.begin());
    for (int i = 0; i < references.size(); ++i)
    {
        EXPECT_EQ(references.at(i), copied.at(i));
    }
}

TEST(TapeReaderIteratorsTest, sunny_scenario_IteratorsSubtractionAsDistanceCounter)
{
    const std::string tape_path =
        TestConfig::GetResourcePath() + "page_size_40_pages_2_records_7.records";
    TapeReader reader(tape_path, 40);
    auto lhs_iter = reader.begin();
    auto rhs_iter = reader.begin();
    int distance = 6;
    for (int i = 0; i < distance; ++i)
    {
        ++rhs_iter;
    }
    EXPECT_EQ(rhs_iter - lhs_iter, distance);
}
