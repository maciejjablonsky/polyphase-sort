#include <gtest/gtest.h>
#include "Record.hpp"

TEST(RecordSerialization, sunny_scenario_SerializeRuntimeRecord)
{
    Record::RuntimeRecord record = {5};
    Record::SerializedRecord serialized = Record::Serialize(record);
    ASSERT_EQ(record.time_since_epoch, serialized.time_since_epoch);
}

TEST(RecordSerialization, sunny_scenario_DeserializeSerializedRecord)
{
    Record::SerializedRecord record = {5};
    Record::RuntimeRecord runtime_record = Record::Deserialize(&record);
    ASSERT_EQ(record.time_since_epoch, runtime_record.time_since_epoch);
}
