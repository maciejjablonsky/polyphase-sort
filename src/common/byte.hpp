#ifndef BYTE_HPP
#define BYTE_HPP

#include <vector>

using byte_vector = std::vector<std::byte>;

inline constexpr std::byte operator "" _b( unsigned long long arg ) noexcept
{
    return static_cast< std::byte >( arg );
}


#endif // BYTE_HPP