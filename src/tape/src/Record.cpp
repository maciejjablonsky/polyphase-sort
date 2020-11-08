#include "Record.hpp"
#include <ctime>
#include <random>

bool Record::operator==(const Record::SerializedRecord &lhs, const Record::SerializedRecord &rhs)
{
    return lhs.creation_time == rhs.creation_time;
}

Record::RuntimeRecord Record::GetRandom()
{
    return {std::rand()};
}

Record::SerializedRecord Record::Serialize(const RuntimeRecord &record)
{
    return {record.creation_time};
}

Record::SerializedRecord Record::Serialize(RuntimeRecord &&record)
{
    return {record.creation_time};
}

Record::RuntimeRecord Record::Deserialize(const SerializedRecord *serialized_record)
{
    return {serialized_record->creation_time};
}
