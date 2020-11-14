#include <gtest/gtest.h>
#include "Page.hpp"
#include <algorithm>

TEST(PageReadingTest, sunny_scenario_ProperRead)
{
    byte_vector vec = {0x1_b, 0x2_b, 0x3_b, 0x4_b, 0x5_b};
    Page page{0x1_b, 0x2_b, 0x3_b, 0x4_b, 0x5_b};
    ASSERT_EQ(page.size(), vec.size());
    for (int i = 0; const auto byte : page)
    {
        ASSERT_EQ(byte, vec[i++]);
    }
}

TEST(PageWritingTest, sunny_scenario_ProperWrite)
{
    byte_vector vec = {0x1_b, 0x2_b, 0x3_b, 0x4_b, 0x5_b};
    Page page(vec.size());
    ASSERT_EQ(page.size(), vec.size());
    for (int i = 0; auto& byte : page)
    {
        byte = vec[i++];
    }
    for (int i = 0; const auto& byte : page)
    {
        ASSERT_EQ(byte, vec[i++]);
    }
}