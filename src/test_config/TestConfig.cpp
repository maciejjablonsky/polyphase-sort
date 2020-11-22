#include "TestConfig.hpp"
#include <chrono>
#include <fmt/format.h>

std::string TestConfig::resource_path_;
std::string TestConfig::tmp_path_;

void TestConfig::Config(const int argc, const char *argv[])
{
    resource_path_ = argv[1];
    resource_path_ += '/';
    tmp_path_ = argv[2];
    tmp_path_ += '/';
}

std::string TestConfig::GetResourcePath()
{
    return resource_path_;
}

std::string TestConfig::GetTmpFilePath()
{
    using namespace std::chrono;
    auto timestamp = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
    return fmt::format("{}/{}.resources", tmp_path_, timestamp);
}

std::string TestConfig::GetTmpDirPath()
{
    return tmp_path_;
}
