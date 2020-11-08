#include <gtest/gtest.h>
#include "PageWriter.hpp"
#include <filetools.hpp>
#include <fmt/format.h>
#include <vector>
#include "TestConfig.hpp"

TEST(PageWriterTest, sunny_scenario_BytesLengthNotExceeding)
{
    const std::string tape_path = TestConfig::GetTmpFilePath(); 
    Page page{0x4_byte, 0_byte, 0_byte, 0_byte, 0x1_byte, 0x2_byte, 0x3_byte, 0x4_byte};
    const int page_size = 8;
    PageWriter writer(tape_path, page_size);
    ASSERT_EQ(page.size(), page_size);
    EXPECT_EQ(page.records_number(), 4);
    EXPECT_NO_THROW(writer.WritePage(page));
}

TEST(PageWriterTest, BytesLengthExceedPageSize)
{
    const std::string tape_path = TestConfig::GetTmpFilePath();
    Page page{0x5_byte, 0_byte, 0_byte, 0_byte, 0x1_byte, 0x2_byte, 0x3_byte, 0x4_byte, 0x5_byte};
    const auto page_size = 8;
    PageWriter writer(tape_path, page_size);
    ASSERT_EQ(page.size(), page_size + 1);
    EXPECT_EQ(page.records_number(), 5);
    EXPECT_ANY_THROW(writer.WritePage(page));
}

TEST(PageWriterTest, DriveAccessCounterWorks)
{
    const std::string tape_path = TestConfig::GetTmpFilePath();
    Page page{0x4_byte, 0_byte, 0_byte, 0_byte, 0x1_byte, 0x2_byte, 0x3_byte, 0x4_byte};
    const auto page_size = 8;
    PageWriter writer(tape_path, page_size);
    ASSERT_EQ(page.size(), page_size);
    
    EXPECT_EQ(writer.GetHardDriveAccessesNumber(), 0);
    writer.WritePage(page);
    EXPECT_EQ(writer.GetHardDriveAccessesNumber(), 1);
}
