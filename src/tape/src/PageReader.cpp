#include "PageReader.hpp"
#include <fmt/format.h>

PageReader::PageReader(const std::string_view in_tape_file_path, const int page_size)
    : in_tape_file_(in_tape_file_path.data(), std::ios::binary | std::ios::ate), page_size_(page_size),
      tape_file_last_byte_pos_([&]() {
          auto pos = in_tape_file_.tellg();
          in_tape_file_.seekg(0, std::ios::beg);
          return pos;
      }())
{
    if (!in_tape_file_.is_open())
    {
        fmt::print("ERROR: Failed to open input tape file at path [{}].\n", in_tape_file_path);
        std::exit(-1);
    }
}

Page PageReader::ReadPage()
{
    Page page(page_size_);
    in_tape_file_.read(reinterpret_cast<char *>(page.data()), page.size());
    ++hard_drive_accesses_;
    return page;
}

bool PageReader::WasLastPageRead() const
{
    return in_tape_file_.tellg() == tape_file_last_byte_pos_;
}

int PageReader::GetHardDriveAccessesNumber() const
{
    return hard_drive_accesses_;
}

int PageReader::PageSize() const
{
    return page_size_;
}

int PageReader::PagesInTapeFile() const
{
    return tape_file_last_byte_pos_ / page_size_;
}
