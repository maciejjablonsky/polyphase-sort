#include <gtest/gtest.h>
#include "PageWriter.hpp"
#include <filetools.hpp>
#include <fmt/format.h>
#include <vector>
#include "TestConfig.hpp"

TEST(PageWriterTest, sunny_scenario_BytesLengthNotExceeding)
{
    const std::string tape_path = TestConfig::GetTmpFilePath();
    Page page{0x4_b, 0_b, 0_b, 0_b, 0_b, 0_b, 0_b, 0_b, 0x1_b, 0x2_b, 0x3_b, 0x4_b};
    const int page_size = 12;
    PageWriter writer(tape_path, page_size);
    ASSERT_EQ(page.size(), page_size);
    EXPECT_EQ(page.records_number(), 4);
    EXPECT_NO_THROW(writer.WritePage(std::move(page)));
}

TEST(PageWriterTest, BytesLengthExceedPageSize)
{
    const std::string tape_path = TestConfig::GetTmpFilePath();
    Page page{0x5_b, 0_b, 0_b, 0_b, 0_b, 0_b, 0_b, 0_b, 0x1_b, 0x2_b, 0x3_b, 0x4_b, 0x5_b};
    const auto page_size = 12;
    PageWriter writer(tape_path, page_size);
    ASSERT_EQ(page.size(), page_size + 1);
    EXPECT_EQ(page.records_number(), 5);
    EXPECT_ANY_THROW(writer.WritePage(std::move(page)));
}

TEST(PageWriterTest, DriveAccessCounterWorks)
{
    const std::string tape_path = TestConfig::GetTmpFilePath();
    Page page{0x4_b, 0_b, 0_b, 0_b, 0_b, 0_b, 0_b, 0_b, 0x1_b, 0x2_b, 0x3_b, 0x4_b};
    const auto page_size = 12;
    PageWriter writer(tape_path, page_size);
    ASSERT_EQ(page.size(), page_size);

    EXPECT_EQ(writer.GetHardDriveAccessesNumber(), 0);
    writer.WritePage(std::move(page));
    writer.Flush();
    EXPECT_EQ(writer.GetHardDriveAccessesNumber(), 1);
}
