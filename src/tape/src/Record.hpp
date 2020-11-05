#ifndef RECORD_HXX
#define RECORD_HXX

#include <vector>
#include <ctime>
#include <type_traits>

namespace Record
{

using memory_vector = std::vector<uint8_t>;

struct SerializedRecord
{
    time_t time_since_epoch;
};

struct RuntimeRecord
{
    time_t time_since_epoch;
};

static_assert(std::is_default_constructible_v<RuntimeRecord>);


RuntimeRecord GetRandom();
SerializedRecord Serialize(const RuntimeRecord &record);
SerializedRecord Serialize(RuntimeRecord &&record);
RuntimeRecord Deserialize(const SerializedRecord * serialized_record);
}; // namespace Record
#endif // RECORD_HXX