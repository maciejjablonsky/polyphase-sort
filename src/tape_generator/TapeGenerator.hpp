#ifndef TAPE_GENERATOR_HPP
#define TAPE_GENERATOR_HPP
#include <optional>
#include <string_view>
#include <vector>
class TapeGenerator {
  public:
    static constexpr auto RECORD_NUM_COMMAND = "--records-number";
    static constexpr auto OUTPUT_FILE_COMMAND = "--out";

  public:
    TapeGenerator(const int argc, const char *argv[]);
    [[nodiscard]] std::optional<int> GetNumberOfRecordsToGenerate() const;
    [[nodiscard]] std::optional<std::string_view> GetOuputFilePath() const;
    void Generate() const;

  private:
    std::vector<std::string_view> argv_;
    int number_of_records_;
    std::string_view out_file_path_;
};

#endif // TAPE_GENERATOR_HPP