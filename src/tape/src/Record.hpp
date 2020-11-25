#ifndef RECORD_HXX
#define RECORD_HXX

#include <vector>
#include <ctime>
#include <type_traits>
#include <compare>
#include <string>
namespace Record
{
struct SerializedRecord
{
    time_t creation_time;
    auto operator<=>(const SerializedRecord& other) const = default;
};

extern const SerializedRecord DEFAULT_MAX;
extern const SerializedRecord DEFAULT_MIN;

struct RuntimeRecord
{
    time_t creation_time;
    void operator+=(time_t offset);
    void operator-=(time_t offset);
    RuntimeRecord operator+(time_t offset);
    RuntimeRecord operator-(time_t offset);
    friend bool operator<(const RuntimeRecord& lhs, const RuntimeRecord& rhs);
};

static_assert(std::is_default_constructible_v<RuntimeRecord>);
static_assert(std::is_aggregate<RuntimeRecord>());
std::string to_string(const SerializedRecord& serialized_record);
RuntimeRecord GetRandom();
SerializedRecord Serialize(const RuntimeRecord& record);
SerializedRecord Serialize(RuntimeRecord&& record);
RuntimeRecord Deserialize(const SerializedRecord* serialized_record);


};     // namespace Record
#endif // RECORD_HXX