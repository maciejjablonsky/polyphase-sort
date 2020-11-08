#include <gtest/gtest.h>
#include "TapeReader.hpp"
#include "TestConfig.hpp"
#include <fmt/format.h>
#include <ranges>

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
}