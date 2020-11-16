#ifndef INFO_HPP
#define INFO_HPP
#include <TapeReader.hpp>
#include <string_view>
#include <vector>

using namespace std::string_view_literals;

class Info
{
    static constexpr auto INPUT_TAPE_PATH_COMMAND = "--in"sv;
    static constexpr auto IS_SORTED_COMMAND = "--is-sorted"sv;
    static constexpr auto COUNT_SERIES_COMMAND = "--count-series"sv;
    static constexpr auto PAGE_SIZE_COMMAND = "--page-size"sv;

  public:
    Info(const int argc, const char* argv[]);
    void PrintUsage() const;
    void Run();

  private:
    void IsSorted() const;
    void CountSeries() const;

  private:
    std::vector<std::string_view> argv_;
    int page_size_ = 4096;
};

#endif // INFO_HPP