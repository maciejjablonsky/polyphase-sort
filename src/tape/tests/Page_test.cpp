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

TEST(PageTest, sunny_scenario_MoveWorks)
{
    byte_vector vec = {0x1_b, 0x2_b, 0x3_b, 0x4_b, 0x5_b};
    Page page(vec.size());
    int i = 0;
    for (auto it = page.begin(); i < 2; ++i, ++it)
    {
        *it = vec[i];
    }
    Page new_page = std::move(page);
    for (auto it = new_page.begin() + i; i < vec.size(); ++i, ++it)
    {
        *it = vec[i];
    }

    for (int i = 0; const auto& byte : new_page)
    {
        ASSERT_EQ(byte, vec[i++]);
    }
}