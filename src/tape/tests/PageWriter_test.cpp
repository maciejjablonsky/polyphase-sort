#include <gtest/gtest.h>
#include "PageWriter.hpp"
#include <filetools.hpp>
#include <fmt/format.h>
#include <vector>

TEST(PageWriterTest, sunny_scenario_BytesLengthNotExceeding)
{
    const std::string tape_path = fmt::format("tmp_{}.records", Timestamp());
    const auto page_size = 4;
    PageWriter writer(tape_path, page_size);
    byte_vector dummy_bytes{0x1_byte, 0x2_byte, 0x3_byte, 0x4_byte};
    ASSERT_EQ(dummy_bytes.size(), page_size);
    EXPECT_NO_THROW(writer.WritePage(dummy_bytes));
}

TEST(PageWriterTest, BytesLengthExceedPageSize)
{
    const std::string tape_path = fmt::format("tmp_{}.records", Timestamp());
    const auto page_size = 4;
    PageWriter writer(tape_path, page_size);
    byte_vector dummy_bytes{0x1_byte, 0x2_byte, 0x3_byte, 0x4_byte, 0x5_byte};
    ASSERT_EQ(dummy_bytes.size(), page_size + 1);
    EXPECT_ANY_THROW(writer.WritePage(dummy_bytes));
}

TEST(PageWriterTest, DriveAccessCounterWorks)
{
    const std::string tape_path = fmt::format("tmp_{}.records", Timestamp());
    const auto page_size = 4;
    PageWriter writer(tape_path, page_size);
    byte_vector dummy_bytes{0x1_byte, 0x2_byte, 0x3_byte, 0x4_byte};
    ASSERT_EQ(dummy_bytes.size(), page_size);
    
    EXPECT_EQ(writer.GetHardDriveAccessesNumber(), 0);
    writer.WritePage(dummy_bytes);
    EXPECT_EQ(writer.GetHardDriveAccessesNumber(), 1);
}
