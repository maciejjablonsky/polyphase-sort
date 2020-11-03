#ifndef TAPE_GENERATOR_HPP
#define TAPE_GENERATOR_HPP
#include <vector>
#include <string_view>
#include <optional>
class TapeGenerator
{
public:
    static constexpr auto RECORD_NUM_COMMAND = "--records-number";
    static constexpr auto OUTPUT_FILE_COMMAND = "--out";

public:
    TapeGenerator(const int argc, const char *argv[]);
    [[nodiscard]] std::optional<int> GetNumberOfRecordsToGenerate() const;
    [[nodiscard]] std::optional<std::string_view> GetOuputFilePath() const;

private:
    std::vector<std::string_view> argv_;
};

#endif // TAPE_GENERATOR_HPP