#include <gtest/gtest.h>
#include "TestConfig.hpp"
#include "PageReader.hpp"
#include "TapeHeaders.hpp"
#include <cstring>

TEST(PageReaderTest, DriveAccessCounterWorks)
{
    const auto in_tape_path = TestConfig::GetResourcePath() + "valid_tape_size_4_records_4.records";
    const int page_size = 8;
    PageReader reader(in_tape_path, page_size);
    EXPECT_EQ(reader.GetHardDriveAccessesNumber(), 0);
    reader.ReadPage();
    EXPECT_EQ(reader.GetHardDriveAccessesNumber(), 1);
}

TEST(PageReaderTest, sunny_scenario_ReadPage)
{
    const auto in_tape_path = TestConfig::GetResourcePath() + "valid_tape_size_4_records_4.records";
    const int page_size = 12;
    PageReader reader(in_tape_path, page_size);
    auto page = reader.ReadPage();
    EXPECT_EQ(page.size(), page_size);
    EXPECT_EQ(page.records_number(), 4);
    Page reference = {0x4_byte, 0_byte, 0_byte, 0_byte,0_byte, 0_byte, 0_byte,0_byte, 0x1_byte, 0x2_byte, 0x3_byte, 0x4_byte};
    EXPECT_TRUE(memcmp(page.data(), reference.data(), page_size) ==
                0);
}

TEST(PageReaderTest, sunny_scenario_PagesInFileNumber)
{
    const auto in_tape_path = TestConfig::GetResourcePath() + "page_size_40_pages_2_records_7.records";
    const int page_size = 40;
    PageReader reader(in_tape_path, page_size);
    EXPECT_EQ(reader.PagesInTapeFile(), 2);
}
