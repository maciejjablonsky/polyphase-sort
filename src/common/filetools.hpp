#ifndef FILETOOLS_HPP
#define FILETOOLS_HPP
#include <vector>
#include <string_view>
#include <iterator>
#include "byte.hpp"
#include <fstream>
namespace
{
template <typename T> std::vector<T> ReadFileWithBinaryArray(const std::string_view file_path)
{
    std::ifstream file(file_path.data(), std::ios::binary);
    if (file.is_open())
    {
        file.seekg(0, file.end);
        int length = file.tellg();
        file.seekg(0, file.beg);
        std::vector<T> vec(length / sizeof(T));
        file.read(reinterpret_cast<char *>(vec.data()), length);
        return vec;
    }
    return {};
}

byte_vector ReadBinaryFile(const std::string_view file_path)
{
    std::ifstream file(file_path.data(), std::ios::binary);
    if (file.is_open())
    {
        file.seekg(0, file.end);
        int length = file.tellg();
        file.seekg(0, file.beg);
        byte_vector vec(length);
        file.read(reinterpret_cast<char *>(vec.data()), length);
        return vec;
    }
    return {};
}

} // namespace

#endif // FILETOOLS_HPP