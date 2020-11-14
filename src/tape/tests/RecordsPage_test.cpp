#include <gtest/gtest.h>
#include "RecordsPage.hpp"

TEST(RecordsPageTest, sunny_scenario_ProperRead)
{
    Page page = {
        0x4_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x1_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b,
        0x0_b, 0x0_b, 0x2_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x3_b, 0x0_b, 0x0_b, 0x0_b,
        0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x4_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b, 0x0_b,
    };
    RecordsPage records_page(std::move(page));
    std::vector<Record::SerializedRecord> records{{1}, {2}, {3}, {4}};
    for (int i = 0; const auto& record : records_page)
    {
        ASSERT_EQ(record, records[i++]);
    }
}

TEST(RecordsPageWritingTest, sunny_scenario_ProperWrite)
{
    std::vector<Record::SerializedRecord> records{{1}, {2}, {3}, {4}};
    Page page(records.size() * sizeof(Record::SerializedRecord) + sizeof(PageHeader));
    RecordsPage records_page(std::move(page));
    std::copy_n(records.cbegin(), records.size(), records_page.begin());
    records_page.WriteStoredRecordsNumber(records.size());
    for (int i = 0; const auto& record : records_page)
    {
        ASSERT_EQ(record, records[i++]);
    }
}