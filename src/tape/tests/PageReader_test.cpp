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
    const int page_size = 8;
    PageReader reader(in_tape_path, page_size);
    auto page = reader.ReadPage();
    EXPECT_EQ(page.size(), page_size);
    const PageHeader *header = reinterpret_cast<const PageHeader *>(page.data());
    EXPECT_EQ(header->inserted_records_number, 4);
    byte_vector reference = {0x1_byte, 0x2_byte, 0x3_byte, 0x4_byte};
    EXPECT_TRUE(memcmp(page.data() + sizeof(PageHeader), reference.data(), page_size - sizeof(PageHeader)) ==
                0);
}