#ifndef TAPE_GENERATOR_HPP
#define TAPE_GENERATOR_HPP
#include <optional>
#include <string_view>
#include <vector>
using namespace std::string_view_literals;
class TapeGenerator {
  public:
    static constexpr auto RECORD_NUM_COMMAND = "--records-number"sv;
    static constexpr auto OUTPUT_FILE_COMMAND = "--out"sv;
    static constexpr auto PAGE_SIZE_COMMAND = "--page-size"sv;
    static constexpr auto SERIES_NUM_COMMAND = "--series"sv;
    static constexpr auto min_records_in_series_ = 3;
    static constexpr auto max_records_in_series_ = 10;

  public:
    TapeGenerator(const int argc, const char *argv[]);
    [[nodiscard]] std::optional<int> GetNumberOfRecordsToGenerate() const;
    [[nodiscard]] std::optional<std::string> GetOuputFilePath() const;
    [[nodiscard]] std::optional<int> GetPageSize() const;
    [[nodiscard]] std::optional<int> GetSeriesNum() const;
    void Generate() const;
    void PrintUsage() const;

  private:
    void GenerateRecords(int records_num) const;
    void GenerateSeries(int series_num) const;
    std::vector<std::string_view> argv_;
    int page_size_ = 4096;
};

#endif // TAPE_GENERATOR_HPP