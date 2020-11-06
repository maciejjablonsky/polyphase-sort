#include "Record.hpp"
#include <ctime>
#include <random>

bool Record::operator==(const Record::SerializedRecord &lhs, const Record::SerializedRecord &rhs)
{
    return lhs.time_since_epoch == rhs.time_since_epoch;
}

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
