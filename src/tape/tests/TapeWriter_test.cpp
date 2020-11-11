#include <gtest/gtest.h>
#include "TapeWriter.hpp"
#include <factory_functions.hpp>
#include <filetools.hpp>
#include <fmt/format.h>
#include "TapeHeaders.hpp"
#include "byte.hpp"
#include "TestConfig.hpp"
#include "TapeReader.hpp"
TEST(TapeWriterTest, sunny_scenario_WriteRecordsToFile)
{
    const std::string path = TestConfig::GetTmpFilePath();
    std::vector<Record::SerializedRecord> reference_records = {
        {0x0111111111111111}, {0x0222222222222222}, {0x0333333333333333}, {0x0444444444444444},
        {0x0555555555555555}, {0x0666666666666666}, {0x0777777777777777}, {0x0888888888888888},
        {0x0999999999999999}, {0x0AAAAAAAAAAAAAAA}};
    // additional scope for RAII
    {
        TapeWriter writer(path, 5 * sizeof(Record::SerializedRecord) + sizeof(PageHeader));

        for (const auto &record : reference_records)
        {
            writer.Write(record);
        }
    }
    auto file_content = ReadBinaryFile(path);
    ASSERT_EQ(file_content.size(), reference_records.size() * sizeof(Record::SerializedRecord) + 2*sizeof(PageHeader));

    TapeReader tape_reader(path, 5 * sizeof(Record::SerializedRecord) + sizeof(PageHeader));
    std::vector<Record::SerializedRecord> records_from_file(reference_records.size());
    std::copy_n(tape_reader.begin(), reference_records.size(), records_from_file.begin());
    EXPECT_EQ(reference_records, records_from_file);
}
