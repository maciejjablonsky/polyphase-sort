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
    const std::string tape_path = TestConfig::GetResourcePath() + "page_size_40_pages_2_records_7.records";
    TapeReader reader(tape_path, 40);
    std::vector<Record::SerializedRecord> references = {{1}, {2}, {3}, {4}, {5}, {6}, {7}};
    int counter = 0;
    for (auto &record : reader)
    {
        EXPECT_EQ(record, references.at(counter));
        counter++;
    }
    EXPECT_EQ(counter, 7);
}

TEST(TapeReaderIteratorsTest, sunny_scenario_CopyAllRecords)
{
    const std::string tape_path = TestConfig::GetResourcePath() + "page_size_40_pages_2_records_7.records";
    TapeReader reader(tape_path, 40);
    std::vector<Record::SerializedRecord> references = {{1}, {2}, {3}, {4}, {5}, {6}, {7}};
    std::vector<Record::SerializedRecord> copied(references.size());
    std::copy_n(reader.begin(), references.size(), copied.begin());
    for (int i = 0; i < references.size(); ++i)
    {
        EXPECT_EQ(references.at(i), copied.at(i));
    }
}

TEST(TapeReaderIteratorsTests, sunny_scenario_CopySomeRecords)
{
    const std::string tape_path = TestConfig::GetResourcePath() + "page_size_40_pages_2_records_7.records";
    TapeReader reader(tape_path, 40);
    std::vector<Record::SerializedRecord> references = {{4}, {5}, {6}};
    std::vector<Record::SerializedRecord> copied(references.size());
    auto find_record = [&](auto &tape, const Record::SerializedRecord &record) {
        auto beginning_ = tape.begin();
        auto ending = TapeReader::sentinel();
        while (beginning_ != ending)
        {
            if ((*beginning_) == record)
            {
                return beginning_;
            }
            ++beginning_;
        }
        return beginning_;
    };
    auto beginning = find_record(reader, {4});
    std::copy_n(beginning, references.size(), copied.begin());
    for (int i = 0; i < references.size(); ++i)
    {
        EXPECT_EQ(references.at(i), copied.at(i));
    }
}

TEST(TapeReaderIteratorsTest, sunny_scenario_IteratorsSubtractionAsDistanceCounter)
{
    const std::string tape_path = TestConfig::GetResourcePath() + "page_size_40_pages_2_records_7.records";
    TapeReader reader(tape_path, 40);
    auto lhs_iter = reader.begin();
    auto rhs_iter = reader.begin();
    int distance = 6;
    for (int i = 0; i < distance; ++i)
    {
        ++lhs_iter;
    }
    EXPECT_EQ(lhs_iter - rhs_iter, distance);
}

TEST(TapeReaderTest, sunny_scenario_ReadFromReadyPages)
{
    Page first = {
        0x4_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x1_b, 0x0_b,
        0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x2_b, 0x0_b, 0x0_b, 0x0_b,
        0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x3_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b,
        0x0_b, 0x0_b, 0x4_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b,
    };
    Page second = {
        0x4_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x5_b, 0x0_b,
        0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x6_b, 0x0_b, 0x0_b, 0x0_b,
        0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x7_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b,
        0x0_b, 0x0_b, 0x8_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b,
    };
    std::vector<Page> pages = {first, second};
    std::vector<Record::SerializedRecord> records = {{1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}};
    TapeReader reader(pages, pages.front().size());
    auto tape_iter = reader.begin();
    auto records_iter = records.begin();
    for (; tape_iter != reader.end() && records_iter != records.end(); ++tape_iter, ++records_iter)
    {
        EXPECT_EQ(*tape_iter, *records_iter); 
    }
    EXPECT_TRUE(tape_iter == reader.end());
    EXPECT_TRUE(records_iter == records.end());
}
