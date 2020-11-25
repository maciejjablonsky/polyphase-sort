#ifndef LOGGER_INTERNAL_HPP
#define LOGGER_INTERNAL_HPP

#include <fstream>
#include <string_view>
#include <chrono>

class LoggerImpl
{
    
   public:
    const static inline std::string DEFAULT_PATH = "output.log";
    LoggerImpl(const std::string_view output_path);
    void Write(const std::string& message);
    void Write(std::string_view message);
    void ChangeOutputPath(const std::string_view output_file_path);

   private:
    void AppendTimestamp();

    
    std::ofstream output_stream_;
};


#endif  // LOGGER_INTERNAL_HPP