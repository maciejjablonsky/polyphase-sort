#include <gtest/gtest.h>
#include "Record.hpp"

TEST(RecordSerializationTest, sunny_scenario_SerializeRuntimeRecord)
{
    Record::RuntimeRecord record = {5};
    Record::SerializedRecord serialized = Record::Serialize(record);
    ASSERT_EQ(record.creation_time, serialized.creation_time);
}

TEST(RecordSerializationTest, sunny_scenario_DeserializeSerializedRecord)
{
    Record::SerializedRecord record = {5};
    Record::RuntimeRecord runtime_record = Record::Deserialize(&record);
    ASSERT_EQ(record.creation_time, runtime_record.creation_time);
}
