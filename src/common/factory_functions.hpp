#ifndef FACTORY_FUNCTIONS_HPP
#define FACTORY_FUNCTIONS_HPP

#include <vector>

namespace funcs
{

template <typename T> int vectorsizeof(const std::vector<T> &vec)
{
    return sizeof(T) * vec.size();
}
} // namespace factory_funcs

#endif // FACTORY_FUNCTIONS_HPP
