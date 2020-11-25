#include "App.hpp"
#include <fmt/format.h>

int main(const int argc, const char* argv[]) {
    try
    {
        App app(argc, argv);
        app.Run();
    }
    catch (const std::exception & e)
    {
        fmt::print("Failed to run app. Error message: {}\n", e.what());
        std::exit(-1);
    }
    return 0;
}
