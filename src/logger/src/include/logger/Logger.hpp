#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string_view>

namespace Logger
{
void Init(const std::string_view log_file_path);

void Dump(const std::string_view message);
}

#endif // LOGGER_HPP