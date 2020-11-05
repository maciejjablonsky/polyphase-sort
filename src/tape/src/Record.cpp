#include "Record.hpp"
#include <ctime>
#include <random>

Record::RuntimeRecord Record::GetRandom()
{
    return {std::rand()};
}

Record::SerializedRecord Record::Serialize(const RuntimeRecord &record)
{
    return {record.time_since_epoch};
}

Record::SerializedRecord Record::Serialize(RuntimeRecord &&record)
{
    return {record.time_since_epoch};
}

Record::RuntimeRecord Record::Deserialize(const SerializedRecord *serialized_record)
{
    return {serialized_record->time_since_epoch};
}
