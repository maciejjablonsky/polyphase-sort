#ifndef DISTRIBUTOR_HPP
#define DISTRIBUTOR_HPP

#include <string_view>
#include <vector>
#include <string>
class Distributor
{
  public:
    Distributor(const std::string_view input_tape_file_path, const int output_tapes_number = 2);

    std::vector<std::string> operator()() const;

  private:
    std::vector<std::string> GenerateOutputTapesPaths() const;

    struct Fibonacci
    {
        int curr;
        int next;
        Fibonacci();     
        int Next();
    } fibonacci;
    const std::string_view input_tape_file_path_;
    const int output_tapes_number_ = 0;
};

#endif // DISTRIBUTOR_HPP