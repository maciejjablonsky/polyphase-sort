#include "Record.hpp"
#include <ctime>
#include <random>
#include <cmath>

bool Record::operator==(const Record::SerializedRecord &lhs, const Record::SerializedRecord &rhs)
{
    return lhs.creation_time == rhs.creation_time;
}

bool Record::operator<(const RuntimeRecord& lhs, const RuntimeRecord& rhs)
{
    return lhs.creation_time < rhs.creation_time;
}

Record::RuntimeRecord Record::GetRandom()
{
    return {std::abs(std::rand())};
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

void Record::RuntimeRecord::operator+=(time_t offset)
{
    creation_time += offset;
}

void Record::RuntimeRecord::operator-=(time_t offset)
{
    creation_time -= offset;
}

Record::RuntimeRecord Record::RuntimeRecord::operator+(time_t offset)
{
    return {this->creation_time + offset};
}

Record::RuntimeRecord Record::RuntimeRecord::operator-(time_t offset)
{
    return {this->creation_time - offset};
}

