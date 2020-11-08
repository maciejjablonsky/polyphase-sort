#ifndef RECORD_HXX
#define RECORD_HXX

#include <vector>
#include <ctime>
#include <type_traits>

namespace Record
{

struct SerializedRecord
{
    time_t creation_time;
};

bool operator==(const SerializedRecord &lhs, const SerializedRecord &rhs);

struct RuntimeRecord
{
    time_t creation_time;
};

static_assert(std::is_default_constructible_v<RuntimeRecord>);


RuntimeRecord GetRandom();
SerializedRecord Serialize(const RuntimeRecord &record);
SerializedRecord Serialize(RuntimeRecord &&record);
RuntimeRecord Deserialize(const SerializedRecord * serialized_record);
}; // namespace Record
#endif // RECORD_HXX