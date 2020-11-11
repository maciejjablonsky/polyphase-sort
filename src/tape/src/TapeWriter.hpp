#ifndef TAPEWRITER_HPP
#define TAPEWRITER_HPP
#include "Record.hpp"
#include "RunView.hpp"
#include <vector>
#include <string_view>
#include <fstream>
#include <queue>

class TapeWriter
{
  public:
    TapeWriter(const std::string_view tape_path, int disk_page_size);
    void Write(RunView && run);
    void Flush();
    ~TapeWriter();

  private:

};

#endif // TAPEWRITER_HPP
