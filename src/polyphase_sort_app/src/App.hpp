#ifndef APP_HPP
#define APP_HPP

#include <vector>
#include <string>
#include <string_view>

class App
{
   public:
    static constexpr auto INPUT_FILE_COMMAND = "--input-from-file";
    static constexpr auto INPUT_KEYBOARD_COMMAND = "--input-from-keyboard";
    static constexpr auto PAGE_SIZE_COMMAND = "--page-size";
    static constexpr auto DISPLAY_SORTED_COMAND = "--display-sorted";
#ifdef ENABLE_LOGGING
    static constexpr auto LOG_OUTPUT_COMMAND = "--log";
#endif  // ENABLE_LOGGING


    App(const int argc, const char* argv[]);
    void PrintUsage() const;
    void Run();

   private:
    void ParseLog(const std::string& path);
    void PrintTape(const std::string& path);
    std::string GetInputPath();
    std::string GetTapeFromKeyboard();
    std::vector<std::string_view> argv_;
    int page_size_ = 4096;
};

#endif APP_HPP