#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <string_view>

namespace Logger
{
void SetOutputPath(const std::string& log_file_path);

void Dump(const std::string & message);
void Dump(std::string_view message);
}

#endif // LOGGER_HPP