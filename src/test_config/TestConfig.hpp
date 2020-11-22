#ifndef TEST_CONFIG_HPP
#define TEST_CONFIG_HPP
#include <string>
class TestConfig
{
  public:
    static void Config(const int argc, const char *argv[]);
    static std::string GetResourcePath();
    static std::string GetTmpFilePath();
    static std::string GetTmpDirPath();

  private:
    static std::string resource_path_;
    static std::string tmp_path_;
};

#endif // TEST_CONFIG_HPP