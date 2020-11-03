#include <exception>
#include <fmt/format.h>
#include "TapeGenerator.hpp"

int main(const int argc, const char *argv[])
{
    try
    {
        TapeGenerator generator(argc, argv);
    }
    catch (const std::exception &e)
    {
        fmt::print("Something wrong happend during generating records. Message: {}\n", e.what());
    }
}