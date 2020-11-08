#include <gtest/gtest.h>
#include "TapeReader.hpp"
#include "TestConfig.hpp"

TEST(TapeReaderTest, sunny_scenario_EndOfTapeReached)
{
    const std::string tape_path = TestConfig::GetResourcePath() + "tape_page_size_36_pages_2_records_7.records";
    TapeReader reader(tape_path, 36);
    for (int i = 0; i < 7; ++i)
    {
        reader.GetNextRecord();
    }
    EXPECT_TRUE(reader.EndOfTape());
}

TEST(TapeReaderTest, sunny_scenario_RecordsAreReadProperly)
{
    const std::string tape_path =
        TestConfig::GetResourcePath() + "tape_page_size_36_pages_2_records_7.records";
    TapeReader reader(tape_path, 36);
    std::vector<Record::SerializedRecord> references = {{1}, {2}, {3}, {4}, {5}, {6}, {7}};
    for (const auto &record : references)
    {
        EXPECT_EQ(record, reader.GetNextRecord());
    }
    EXPECT_ANY_THROW(reader.GetNextRecord());
}