#include <exception>
#include <fmt/format.h>
#include "Info.hpp"

int main(const int argc, const char *argv[])
{
    try
    {
        Info info(argc, argv);
        info.Run();
    }
    catch (const std::exception &e)
    {
        fmt::print("Something wrong. Message: {}\n", e.what());
    }
}