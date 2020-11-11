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

  public:
    TapeGenerator(const int argc, const char *argv[]);
    [[nodiscard]] std::optional<int> GetNumberOfRecordsToGenerate() const;
    [[nodiscard]] std::optional<std::string> GetOuputFilePath() const;
    [[nodiscard]] std::optional<int> GetPageSize() const;
    void Generate() const;

  private:
    std::vector<std::string_view> argv_;
    int number_of_records_;
    std::string out_file_path_;
    int page_size_ = 4096;
};

#endif // TAPE_GENERATOR_HPP