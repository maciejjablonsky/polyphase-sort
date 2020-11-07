#include <gtest/gtest.h>
#include "TapeWriter.hpp"
#include <factory_functions.hpp>
#include <filetools.hpp>
#include <fmt/format.h>
#include "TapeHeaders.hpp"

using byte_vector = std::vector<std::byte>;



TEST(TapeWriterTest, sunny_scenario_WriteRecordsToFile)
{
    const std::string path = fmt::format("tmp_{}.records", Timestamp());

    // additional scope for RAII
    {
        TapeWriter writer(path, 4096);

        writer.Write({{5}, {4}, {3}, {2}, {1}});
        writer.Write({{5}, {4}, {3}, {2}, {1}});
    }
    std::vector<Record::SerializedRecord> reference_records = {{5}, {4}, {3}, {2}, {1},
                                                               {5}, {4}, {3}, {2}, {1}};
    auto records_from_file = ReadFileWithBinaryArray<Record::SerializedRecord>(path);
    ASSERT_EQ(records_from_file.size(), reference_records.size());
    EXPECT_EQ(records_from_file, reference_records);
}
